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
__global__ void render_kernel(double* img, int width, int height, Object** scene_objects, int scene_size, 
                              Camera camera, int samples_per_pixel, int max_depth, unsigned int seed) {
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
        pixelColor += calculateColor(ray, scene_objects, scene_size, max_depth, thread_seed);
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
__device__ Vec3 calculateColor(const Ray& ray, Object** scene_objects, int scene_size, int depth, unsigned int& seed) {
    if (depth <= 0) {
        return Vec3(0, 0, 0);
    }
    
    HitInfo hitInfo;
    hitInfo.distance = DBL_MAX;
    hitInfo.hitObject = nullptr;
    
    // Find closest intersection
    for (int i = 0; i < scene_size; i++) {
        Vec3 intersection, normal;
        double t;
        if (scene_objects[i]->intersect(ray, 0.001, hitInfo.distance, intersection, t, normal)) {
            hitInfo.hitObject = scene_objects[i];
            hitInfo.distance = t;
            hitInfo.intersection = intersection;
            hitInfo.normal = normal;
            hitInfo.setFaceNormal(ray, normal);
        }
    }
    
    if (hitInfo.hitObject) {
        Ray scattered;
        Vec3 attenuation;
        if (hitInfo.hitObject->material->scatter(ray, hitInfo, attenuation, scattered)) {
            return attenuation * calculateColor(scattered, scene_objects, scene_size, depth - 1, seed);
        }
        return Vec3(0, 0, 0);
    }
    
    // Background color
    Vec3 unitDirection = ray.direction.normalize();
    double t = (unitDirection.y + 1.0) / 2;
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
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
    
    // Prepare scene objects for GPU
    Object** d_scene_objects;
    cudaMalloc(&d_scene_objects, scene.size() * sizeof(Object*));
    
    // Copy scene objects to GPU (this is simplified - in practice, you'd need to handle the polymorphic objects properly)
    // This would require a more complex implementation to handle the object hierarchy
    cudaMemcpy(d_scene_objects, scene.data(), scene.size() * sizeof(Object*), cudaMemcpyHostToDevice);
    
    // Set up CUDA grid and blocks
    dim3 block(16, 16);
    dim3 grid((width + block.x - 1) / block.x, (height + block.y - 1) / block.y);
    
    // Generate random seed
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    // Launch CUDA kernel
    render_kernel<<<grid, block>>>(d_img, width, height, d_scene_objects, scene.size(), 
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
    cudaFree(d_scene_objects);
    
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