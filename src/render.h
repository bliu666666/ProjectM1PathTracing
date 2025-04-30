#pragma once

#include "camera.h"
#include "../ppm_writer/ppm_writer.h"
#include "lambertian.h"
#include "specular.h"
#include "glossy.h"
#include "dielectric.h"
#include "mlt_path.h"

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

// Integrated MLT rendering engine
void renderMLT(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
               const Vec3& v_up,double v_fov,int samples_per_pixel,int max_depth,int num_iterations)
{

    double aspect=width/height;
    Camera camera(origin,lookat,v_up,v_fov, aspect);
    double* img=new double[static_cast<int>(width)*static_cast<int>(height)*3];

    int total_rows = static_cast<int>(height);

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

                // Call the MLT main function to perform multiple sampling on a single path
                Vec3 result=metropolisRender(scene,camera,max_depth,num_iterations); // Adjustable iterations

                pixelColor+=result;
            }

            pixelColor=pixelColor/static_cast<double>(samples_per_pixel);

            // Clamp to [0,1] before writing
            pixelColor.x=std::min(1.0,std::max(0.0,pixelColor.x));
            pixelColor.y=std::min(1.0,std::max(0.0,pixelColor.y));
            pixelColor.z=std::min(1.0,std::max(0.0,pixelColor.z));

            // Gamma correction and write to image buffer
            int index=3*(i*static_cast<int>(width)+j);
            img[index+0]=pixelColor.x; 
            img[index+1]=pixelColor.y;
            img[index+2]=pixelColor.z;
        }

        // real-time progress display output the current completion percentage every 5 lines rendered
        if (i % 5 == 0) {
            int progress = static_cast<int>(100.0 * i / total_rows);
            #pragma omp critical
            std::cout << "[Progress] " << progress << "%" <<"completed" << std::endl;
        }
    }

    // Write image
    writePPM_MLT(outputPath,static_cast<unsigned>(width),static_cast<unsigned>(height),img);
    delete[] img;
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