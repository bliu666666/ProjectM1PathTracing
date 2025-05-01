#pragma once

#include "camera.h"
#include "../ppm_writer/ppm_writer.h"
#include "lambertian.h"
#include "specular.h"
#include "glossy.h"
#include "dielectric.h"
#include "mlt_path.h"
#include "cuda_compat.h"

#if CUDA_ENABLED
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#endif

// Réduction OpenMP pour Vec3
#pragma omp declare reduction(+ : Vec3 : omp_out = omp_out + omp_in) initializer(omp_priv = Vec3())

// Calcule la couleur d'un point en fonction des intersections du rayon
// Si le rayon touche un objet, calcule la couleur basée sur le matériau
// Sinon, retourne la couleur du fond (ciel)
Vec3 calculateColor(const Ray& ray, const std::vector<Object*>& scene,int depth)
{
    if (depth <= 0) {
        return Vec3(0,0,0); // Dépassement de la profondeur maximale, retourne noir
    }
    HitInfo hitInfo=findFirstCollision(ray,scene);
    if (hitInfo.hitObject)
    {
        Ray scattered;
        Vec3 attenuation;
        // Si le matériau diffuse la lumière, continue le traçage
        if (hitInfo.hitObject->material->scatter(ray,hitInfo,attenuation,scattered))
        {
            return attenuation*calculateColor(scattered, scene, depth - 1);
        }
        return Vec3(0,0,0);// Si le matériau ne diffuse pas, retourne noir
    }
    // Couleur de fond en dégradé
    Vec3 unitDirection=ray.direction.normalize();
    double t=(unitDirection.y+1.0)/2;
    return (1.0-t)*Vec3(1.0,1.0,1.0)+t*Vec3(0.5,0.7,1.0);
}

// Fonction principale de rendu
// Parcourt chaque pixel et calcule sa couleur par échantillonnage multiple
void render(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
            const Vec3& v_up, double v_fov,int samples_per_pixel,int max_depth)
{
    double aspect=width/height;
    // Utilise les paramètres spécifiés par l'utilisateur pour la caméra
    Camera camera(origin,lookat,v_up,v_fov,aspect);
    double *img=new double[static_cast<int>(width)*static_cast<int>(height)*3];

    // Parcourt chaque pixel et calcule sa couleur
    // OpenMP parallélise la boucle externe
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i=static_cast<int>(height)-1;i>=0;--i)
    {
        for (int j=0;j<static_cast<int>(width);++j)
        {
            Vec3 color(0,0,0);
            std::vector<Vec3> thread_colors(omp_get_max_threads(),Vec3(0,0,0));
            // La boucle interne est également parallèle + réduction
            #pragma omp parallel
            {
                int tid=omp_get_thread_num();
                #pragma omp single nowait
                {            
                    for (int s=0;s<samples_per_pixel;++s) 
                    {
                        #pragma omp task firstprivate(tid)
                        {
                            double u=(j+randomDouble())/width;
                            double v=(i+randomDouble())/height;
                            Ray ray=camera.getRay(u, v);
                            Vec3 col=calculateColor(ray,scene,max_depth);
                            thread_colors[tid]=thread_colors[tid]+col;
                        }
                    }
                }
            }

            for (const auto& c:thread_colors) 
            {
                color=color+c;
            }

            color=color/static_cast<double>(samples_per_pixel);// Moyenne des échantillons

            // Conversion des couleurs dans l'intervalle [0,255]
            img[3*(i*static_cast<int>(width)+j)+0]=sqrt(color.x);// Correction gamma
            img[3*(i*static_cast<int>(width)+j)+1]=sqrt(color.y);
            img[3*(i*static_cast<int>(width)+j)+2]=sqrt(color.z);
        }
    }
    unsigned char* rgbImg =img2rgb(width,height,img);
    writePPM_normal(outputPath,width,height,rgbImg);
    delete []img;
    free(rgbImg);
}

// CUDA kernel for path tracing
#if CUDA_ENABLED

// Enumération pour les types d'objets
enum ObjectType {
    TYPE_SPHERE = 0,
    TYPE_AABB = 1
};

// Enumération pour les types de matériaux
enum MaterialType {
    MAT_LAMBERTIAN = 0,
    MAT_SPECULAR = 1,
    MAT_GLOSSY = 2,
    MAT_DIELECTRIC = 3,
    MAT_EMISSIVE = 4
};

__global__ void render_kernel(double* img, int width, int height, int* obj_types, int scene_size,Vec3* sphere_centers, double* sphere_radii,Vec3* aabb_min, Vec3* aabb_max,int* mat_types, Vec3* mat_colors, double* mat_params,Camera camera, int samples_per_pixel, int max_depth, unsigned int seed) {
    // Calculate pixel coordinates
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (i >= height || j >= width) return;
    
    // Each thread gets a unique seed
    unsigned int thread_seed = seed + i * width + j;
    
    Vec3 pixelColor(0, 0, 0);
    
    for (int s = 0; s < samples_per_pixel; s++) {
        // Generate random offsets for anti-aliasing
        double u = (j + randomDoubleThread(thread_seed)) / width;
        double v = (i + randomDoubleThread(thread_seed)) / height;
        
        // Generate ray from camera
        Ray ray = camera.getRay(u, v);
        
        // Trace path and accumulate color
        pixelColor += calculateColor(ray, obj_types, sphere_centers, sphere_radii, 
                                    aabb_min, aabb_max, mat_types, mat_colors, mat_params,
                                    scene_size, max_depth, thread_seed);
    }
    
    // Average samples and apply gamma correction
    pixelColor = pixelColor / samples_per_pixel;
    pixelColor = Vec3(sqrt(pixelColor.x), sqrt(pixelColor.y), sqrt(pixelColor.z));
    
    // Store result in the image buffer
    int idx = (i * width + j) * 3;
    img[idx] = pixelColor.x;
    img[idx + 1] = pixelColor.y;
    img[idx + 2] = pixelColor.z;
}

// Helper function to calculate color for CUDA kernel
__device__ Vec3 calculateColor(const Ray& ray, int* obj_types, 
                              Vec3* sphere_centers, double* sphere_radii,
                              Vec3* aabb_min, Vec3* aabb_max,
                              int* mat_types, Vec3* mat_colors, double* mat_params,
                              int scene_size, int depth, unsigned int& seed) {
    if (depth <= 0) {
        return Vec3(0, 0, 0);
    }
    
    // Initialisation des données d'intersection
    double closest_t = DBL_MAX;
    Vec3 hit_point, hit_normal;
    int hit_obj_idx = -1;
    int hit_mat_type = -1;
    
    // Find closest intersection
    for (int i = 0; i < scene_size; i++) {
        bool hit = false;
        Vec3 intersection, normal;
        double t;
        
        // Intersection avec le type d'objet approprié
        if (obj_types[i] == TYPE_SPHERE) {
            // Intersection avec une sphère
            Vec3 center = sphere_centers[i];
            double radius = sphere_radii[i];
            
            Vec3 oc = ray.origin - center;
            double a = ray.direction.dot(ray.direction);
            double b = 2.0 * oc.dot(ray.direction);
            double c = oc.dot(oc) - radius * radius;
            double discriminant = b * b - 4 * a * c;
            
            if (discriminant > 0) {
                double root = (-b - sqrt(discriminant)) / (2.0 * a);
                if (root < 0.001 || root >= closest_t) {
                    root = (-b + sqrt(discriminant)) / (2.0 * a);
                    if (root < 0.001 || root >= closest_t) {
                        continue;
                    }
                }
                
                t = root;
                intersection = ray.at(t);
                normal = (intersection - center) / radius;
                hit = true;
            }
        } else if (obj_types[i] == TYPE_AABB) {
            // Intersection avec une AABB
            Vec3 min_point = aabb_min[i];
            Vec3 max_point = aabb_max[i];
            
            double tx1 = (min_point.x - ray.origin.x) / ray.direction.x;
            double tx2 = (max_point.x - ray.origin.x) / ray.direction.x;
            double ty1 = (min_point.y - ray.origin.y) / ray.direction.y;
            double ty2 = (max_point.y - ray.origin.y) / ray.direction.y;
            double tz1 = (min_point.z - ray.origin.z) / ray.direction.z;
            double tz2 = (max_point.z - ray.origin.z) / ray.direction.z;
            
            double tmin = fmax(fmax(fmin(tx1, tx2), fmin(ty1, ty2)), fmin(tz1, tz2));
            double tmax = fmin(fmin(fmax(tx1, tx2), fmax(ty1, ty2)), fmax(tz1, tz2));
            
            if (tmax < 0 || tmin > tmax) {
                continue;
            }
            
            t = tmin > 0.001 ? tmin : tmax;
            if (t < 0.001 || t >= closest_t) {
                continue;
            }
            
            intersection = ray.at(t);
            
            // Déterminer la normale en fonction de la face touchée
            Vec3 center = (min_point + max_point) * 0.5;
            Vec3 extents = max_point - min_point;
            Vec3 d = intersection - center;
            Vec3 abs_d = Vec3(fabs(d.x), fabs(d.y), fabs(d.z));
            
            if (abs_d.x > abs_d.y && abs_d.x > abs_d.z) {
                normal = Vec3(d.x > 0 ? 1 : -1, 0, 0);
            } else if (abs_d.y > abs_d.z) {
                normal = Vec3(0, d.y > 0 ? 1 : -1, 0);
            } else {
                normal = Vec3(0, 0, d.z > 0 ? 1 : -1);
            }
            
            hit = true;
        }
        
        if (hit && t < closest_t) {
            closest_t = t;
            hit_point = intersection;
            hit_normal = normal;
            hit_obj_idx = i;
            hit_mat_type = mat_types[i];
            
            // Assurer que la normale pointe contre le rayon
            if (ray.direction.dot(normal) > 0) {
                hit_normal = -hit_normal;
            }
        }
    }
    
    if (hit_obj_idx >= 0) {
        // Traitement du matériau
        Ray scattered;
        Vec3 attenuation;
        bool scattered_ray = false;
        
        // Récupérer la couleur du matériau
        Vec3 mat_color = mat_colors[hit_obj_idx];
        
        // Traiter selon le type de matériau
        if (mat_types[hit_obj_idx] == MAT_LAMBERTIAN) {
            // Matériau lambertien (diffus)
            Vec3 scatter_direction = hit_normal + randomUnitVector(seed);
            if (scatter_direction.near_zero()) {
                scatter_direction = hit_normal;
            }
            scattered = Ray(hit_point, scatter_direction);
            attenuation = mat_color;
            scattered_ray = true;
        } else if (mat_types[hit_obj_idx] == MAT_SPECULAR) {
            // Matériau spéculaire (miroir)
            Vec3 reflected = reflect(ray.direction.normalize(), hit_normal);
            scattered = Ray(hit_point, reflected);
            attenuation = mat_color;
            scattered_ray = (scattered.direction.dot(hit_normal) > 0);
        } else if (mat_types[hit_obj_idx] == MAT_GLOSSY) {
            // Matériau glossy (miroir avec rugosité)
            double roughness = mat_params[hit_obj_idx];
            Vec3 reflected = reflect(ray.direction.normalize(), hit_normal);
            Vec3 random_dir = randomInUnitSphere(seed);
            scattered = Ray(hit_point, reflected + roughness * random_dir);
            attenuation = mat_color;
            scattered_ray = (scattered.direction.dot(hit_normal) > 0);
        } else if (mat_types[hit_obj_idx] == MAT_DIELECTRIC) {
            // Matériau diélectrique (verre, eau)
            attenuation = Vec3(1.0, 1.0, 1.0);
            double refraction_ratio = ray.direction.dot(hit_normal) < 0 
                                    ? 1.0 / mat_params[hit_obj_idx] 
                                    : mat_params[hit_obj_idx];
            
            Vec3 unit_direction = ray.direction.normalize();
            double cos_theta = fmin(-unit_direction.dot(hit_normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
            
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vec3 direction;
            
            if (cannot_refract || schlick(cos_theta, refraction_ratio) > randomDoubleThread(seed)) {
                direction = reflect(unit_direction, hit_normal);
            } else {
                direction = refract(unit_direction, hit_normal, refraction_ratio);
            }
            
            scattered = Ray(hit_point, direction);
            scattered_ray = true;
        } else if (mat_types[hit_obj_idx] == MAT_EMISSIVE) {
            // Matériau émissif (source de lumière)
            return mat_color;
        }
        
        if (scattered_ray) {
            return attenuation * calculateColor(scattered, obj_types, sphere_centers, sphere_radii,
                                              aabb_min, aabb_max, mat_types, mat_colors, mat_params,
                                              scene_size, depth - 1, seed);
        }
        return Vec3(0, 0, 0);
    }
    
    // Background color
    Vec3 unitDirection = ray.direction.normalize();
    double t = (unitDirection.y + 1.0) / 2;
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

// Fonctions utilitaires pour le kernel CUDA
__device__ Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * v.dot(n) * n;
}

__device__ Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    double cos_theta = fmin(-uv.dot(n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.dot(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

__device__ double schlick(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

__device__ Vec3 randomUnitVector(unsigned int& seed) {
    double a = 2.0 * M_PI * randomDoubleThread(seed);
    double z = 2.0 * randomDoubleThread(seed) - 1.0;
    double r = sqrt(1.0 - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

__device__ Vec3 randomInUnitSphere(unsigned int& seed) {
    while (true) {
        Vec3 p = Vec3(randomDoubleThread(seed) * 2 - 1, 
                      randomDoubleThread(seed) * 2 - 1, 
                      randomDoubleThread(seed) * 2 - 1);
        if (p.dot(p) < 1) return p;
    }
}
#endif

// Integrated CUDA rendering engine
void render_cuda(double width, double height, const std::vector<Object*>& scene, char* outputPath, const Vec3& origin, const Vec3& lookat,
                 const Vec3& v_up, double v_fov, int samples_per_pixel, int max_depth) {
#if CUDA_ENABLED
    double aspect = width / height;
    Camera camera(origin, lookat, v_up, v_fov, aspect);
    
    // Allocate host memory for the image
    double* h_img = new double[static_cast<int>(width) * static_cast<int>(height) * 3];
    
    // Allocate device memory for the image
    double* d_img;
    cudaMalloc(&d_img, static_cast<int>(width) * static_cast<int>(height) * 3 * sizeof(double));
    
    // Préparation des données SoA pour la scène
    int scene_size = scene.size();
    
    // Allocation de la mémoire hôte (pinned) pour les tableaux SoA
    int* h_obj_types;
    Vec3* h_sphere_centers;
    double* h_sphere_radii;
    Vec3* h_aabb_min;
    Vec3* h_aabb_max;
    int* h_mat_types;
    Vec3* h_mat_colors;
    double* h_mat_params;
    
    cudaHostAlloc(&h_obj_types, scene_size * sizeof(int), cudaHostAllocDefault);
    cudaHostAlloc(&h_sphere_centers, scene_size * sizeof(Vec3), cudaHostAllocDefault);
    cudaHostAlloc(&h_sphere_radii, scene_size * sizeof(double), cudaHostAllocDefault);
    cudaHostAlloc(&h_aabb_min, scene_size * sizeof(Vec3), cudaHostAllocDefault);
    cudaHostAlloc(&h_aabb_max, scene_size * sizeof(Vec3), cudaHostAllocDefault);
    cudaHostAlloc(&h_mat_types, scene_size * sizeof(int), cudaHostAllocDefault);
    cudaHostAlloc(&h_mat_colors, scene_size * sizeof(Vec3), cudaHostAllocDefault);
    cudaHostAlloc(&h_mat_params, scene_size * sizeof(double), cudaHostAllocDefault);
    
    // Remplir les tableaux SoA à partir de la scène
    for (int i = 0; i < scene_size; i++) {
        Object* obj = scene[i];
        
        // Déterminer le type d'objet et extraire ses attributs
        if (Sphere* sphere = dynamic_cast<Sphere*>(obj)) {
            h_obj_types[i] = TYPE_SPHERE;
            h_sphere_centers[i] = sphere->center;
            h_sphere_radii[i] = sphere->r;
            // Valeurs par défaut pour AABB
            h_aabb_min[i] = Vec3(0, 0, 0);
            h_aabb_max[i] = Vec3(0, 0, 0);
        } else if (AABB* aabb = dynamic_cast<AABB*>(obj)) {
            h_obj_types[i] = TYPE_AABB;
            h_aabb_min[i] = aabb->min;
            h_aabb_max[i] = aabb->max;
            // Valeurs par défaut pour Sphere
            h_sphere_centers[i] = Vec3(0, 0, 0);
            h_sphere_radii[i] = 0;
        }
        
        // Déterminer le type de matériau et extraire ses attributs
        Material* mat = obj->material;
        
        if (Lambertian* lambertian = dynamic_cast<Lambertian*>(mat)) {
            h_mat_types[i] = MAT_LAMBERTIAN;
            h_mat_colors[i] = lambertian->albedo;
            h_mat_params[i] = 0.0; // Non utilisé pour Lambertian
        } else if (Specular* specular = dynamic_cast<Specular*>(mat)) {
            h_mat_types[i] = MAT_SPECULAR;
            h_mat_colors[i] = specular->albedo;
            h_mat_params[i] = 0.0; // Non utilisé pour Specular
        } else if (Glossy* glossy = dynamic_cast<Glossy*>(mat)) {
            h_mat_types[i] = MAT_GLOSSY;
            h_mat_colors[i] = glossy->albedo;
            h_mat_params[i] = glossy->roughness;
        } else if (Dielectric* dielectric = dynamic_cast<Dielectric*>(mat)) {
            h_mat_types[i] = MAT_DIELECTRIC;
            h_mat_colors[i] = Vec3(1.0, 1.0, 1.0); // Dielectric est généralement transparent
            h_mat_params[i] = dielectric->ref_idx;
        } else if (Emissive* emissive = dynamic_cast<Emissive*>(mat)) {
            h_mat_types[i] = MAT_EMISSIVE;
            h_mat_colors[i] = emissive->emission;
            h_mat_params[i] = 0.0; // Non utilisé pour Emissive
        }
    }
    
    // Allocation de la mémoire device pour les tableaux SoA
    int* d_obj_types;
    Vec3* d_sphere_centers;
    double* d_sphere_radii;
    Vec3* d_aabb_min;
    Vec3* d_aabb_max;
    int* d_mat_types;
    Vec3* d_mat_colors;
    double* d_mat_params;
    
    cudaMalloc(&d_obj_types, scene_size * sizeof(int));
    cudaMalloc(&d_sphere_centers, scene_size * sizeof(Vec3));
    cudaMalloc(&d_sphere_radii, scene_size * sizeof(double));
    cudaMalloc(&d_aabb_min, scene_size * sizeof(Vec3));
    cudaMalloc(&d_aabb_max, scene_size * sizeof(Vec3));
    cudaMalloc(&d_mat_types, scene_size * sizeof(int));
    cudaMalloc(&d_mat_colors, scene_size * sizeof(Vec3));
    cudaMalloc(&d_mat_params, scene_size * sizeof(double));
    
    // Copier les données de l'hôte vers le device
    cudaMemcpy(d_obj_types, h_obj_types, scene_size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sphere_centers, h_sphere_centers, scene_size * sizeof(Vec3), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sphere_radii, h_sphere_radii, scene_size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_aabb_min, h_aabb_min, scene_size * sizeof(Vec3), cudaMemcpyHostToDevice);
    cudaMemcpy(d_aabb_max, h_aabb_max, scene_size * sizeof(Vec3), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mat_types, h_mat_types, scene_size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mat_colors, h_mat_colors, scene_size * sizeof(Vec3), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mat_params, h_mat_params, scene_size * sizeof(double), cudaMemcpyHostToDevice);
    
    // Set up CUDA grid and blocks
    dim3 block(16, 16);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);
    
    // Generate random seed
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    // Launch CUDA kernel
    render_kernel<<<grid, block>>>(d_img, width, height, 
                                  d_obj_types, scene_size,
                                  d_sphere_centers, d_sphere_radii,
                                  d_aabb_min, d_aabb_max,
                                  d_mat_types, d_mat_colors, d_mat_params,
                                  camera, samples_per_pixel, max_depth, seed);
    
    // Copy result back to host
    cudaMemcpy(h_img, d_img, static_cast<int>(width) * static_cast<int>(height) * 3 * sizeof(double), cudaMemcpyDeviceToHost);
    
    // Convert to RGB and write to file
    unsigned char* rgbImg = img2rgb(width, height, h_img);
    writePPM_normal(outputPath, width, height, rgbImg);
    
    // Free memory
    delete[] h_img;
    free(rgbImg);
    cudaFree(d_img);
    
    // Libérer la mémoire device pour les tableaux SoA
    cudaFree(d_obj_types);
    cudaFree(d_sphere_centers);
    cudaFree(d_sphere_radii);
    cudaFree(d_aabb_min);
    cudaFree(d_aabb_max);
    cudaFree(d_mat_types);
    cudaFree(d_mat_colors);
    cudaFree(d_mat_params);
    
    // Libérer la mémoire hôte (pinned) pour les tableaux SoA
    cudaFreeHost(h_obj_types);
    cudaFreeHost(h_sphere_centers);
    cudaFreeHost(h_sphere_radii);
    cudaFreeHost(h_aabb_min);
    cudaFreeHost(h_aabb_max);
    cudaFreeHost(h_mat_types);
    cudaFreeHost(h_mat_colors);
    cudaFreeHost(h_mat_params);
    
    std::cout << "CUDA rendering completed." << std::endl;
#else
    std::cout << "CUDA rendering not available. Compile with CUDA_ENABLED=1 to enable." << std::endl;
    // Fall back to CPU rendering
    render(width, height, scene, outputPath, origin, lookat, v_up, v_fov, samples_per_pixel, max_depth);
#endif
}
// Integrated MLT rendering engine
void renderMLT(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
               const Vec3& v_up,double v_fov,int samples_per_pixel,int max_depth,int num_iterations)
{
    double aspect=width/height;
    Camera camera(origin,lookat,v_up,v_fov, aspect);
    double* img=new double[static_cast<int>(width)*static_cast<int>(height)*3];

    int total_rows = static_cast<int>(height);
    
    // Réduire le nombre d'itérations par pixel pour éviter les calculs excessifs
    int iterations_per_pixel = std::max(10, num_iterations / 100);
    std::cout << "Using " << iterations_per_pixel << " MLT iterations per pixel (reduced from " << num_iterations << ")" << std::endl;

    #pragma omp parallel for schedule(dynamic)
    for (int i=0;i<static_cast<int>(height);++i)
    {
        for (int j=0;j<static_cast<int>(width);++j)
        {
            Vec3 pixelColor(0,0,0);

            // Each pixel (i,j) has a unique seed
            unsigned int thread_seed=std::chrono::system_clock::now().time_since_epoch().count()+i*static_cast<int>(width)+j;

            for (int s=0;s<samples_per_pixel;++s)
            {
                // Normalize the u, v range of each pixel
                double u=(j+randomDoubleThread(thread_seed))/width;
                double v=(i+randomDoubleThread(thread_seed))/height;

                // Set up camera light for MLT initialization
                Ray ray=camera.getRay(u,v);

                // Call the MLT main function with REDUCED iterations
                Vec3 result=metropolisRender(scene,camera,max_depth,iterations_per_pixel);

                pixelColor+=result;
            }

        

std::vector<Object*> createScene() 
{
    // Material Definition
    Lambertian* red_wall = new Lambertian(Vec3(0.65, 0.05, 0.05));
    Lambertian* green_wall = new Lambertian(Vec3(0.12, 0.45, 0.15));
    Lambertian* white_wall = new Lambertian(Vec3(0.73, 0.73, 0.73));
    Lambertian* white_floor = new Lambertian(Vec3(0.73, 0.73, 0.73));

    Lambertian* red_material = new Lambertian(Vec3(0.8,0.3,0.3));
    Specular* metal_material = new Specular(Vec3(0.8, 0.8, 0.8));
    Glossy* glossy_material = new Glossy(Vec3(0.9, 0.6, 0.2), 0.3);
    Dielectric* glass_material = new Dielectric(1.5);
    Emissive* light = new Emissive(Vec3(10.0, 10.0, 10.0));

    std::vector<Object*> scene;

    // light source
    scene.push_back(new Sphere(light, 1.0, Vec3(0, 1.2, -2)));

    // Wall and floor
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.0, -4.0), Vec3(2.0, 2.0, -3.99))); // Back Wall
    scene.push_back(new AABB(green_wall, Vec3(-2.0, -2.0, -4.0), Vec3(-1.99, 2.0, 0.0)));  // Left Wall
    scene.push_back(new AABB(red_wall, Vec3(1.99, -2.0, -4.0), Vec3(2.0, 2.0, 0.0)));      // Right Wall
    scene.push_back(new AABB(white_floor, Vec3(-2.0, -2.01, -4.0), Vec3(2.0, -2.0, 0.0))); // ground
    scene.push_back(new AABB(white_wall, Vec3(-2.0, 2.0, -4.0), Vec3(2.0, 2.01, 0.0)));    // ceiling

    // Spheres of various materials
    scene.push_back(new Sphere(metal_material, 0.4, Vec3(-1.2, -1.6, -2.5)));
    scene.push_back(new Sphere(glossy_material, 0.4, Vec3(-0.4, -1.6, -2.5)));
    scene.push_back(new Sphere(red_material, 0.4, Vec3(0.4, -1.6, -2.5)));
    scene.push_back(new Sphere(glass_material, 0.4, Vec3(1.2, -1.6, -2.5)));

    return scene;
}

/*
    A simple scene used only for performance testing, 
    with very few light bounces and light CPU load, 
    very suitable for performance testing
*/
std::vector<Object*> createTestScene()
{
    std::vector<Object*> scene;
    Emissive* light = new Emissive(Vec3(8.0, 8.0, 8.0));
    Lambertian* floor_material = new Lambertian(Vec3(0.7, 0.7, 0.7));

    scene.push_back(new Sphere(light, 0.8, Vec3(0, 1, -3)));  // 小光源
    scene.push_back(new AABB(floor_material, Vec3(-5, -1, -5), Vec3(5, 0, 5))); // 地板

    return scene;
}