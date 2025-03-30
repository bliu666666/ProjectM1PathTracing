#pragma once

#include "firstCollision.h"
#include "camera.h"
#include "../ppm_writer/ppm_writer.c"
#include "lambertian.h"
#include <chrono>
#include <omp.h>

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
    #pragma omp parallel for schedule(dynamic)
    for (int i=height-1;i>=0;--i)
    {
        for (int j=0;j<width;++j)
        {
            Vec3 color(0,0,0);
            // La boucle interne est également parallèle + réduction
            #pragma omp parallel for reduction(+:color) schedule(dynamic)
            for (int s=0;s<samples_per_pixel;++s) {
                double u=(j+randomDouble())/width;
                double v=(i+randomDouble())/height;
                Ray ray=camera.getRay(u, v);
                color=color+calculateColor(ray,scene,max_depth);
            }
            color=color/static_cast<double>(samples_per_pixel);// Moyenne des échantillons

            // Conversion des couleurs dans l'intervalle [0,255]
            img[3*(i*static_cast<int>(width)+j)+0]=sqrt(color.x);// Correction gamma
            img[3*(i*static_cast<int>(width)+j)+1]=sqrt(color.y);
            img[3*(i*static_cast<int>(width)+j)+2]=sqrt(color.z);
        }
    }
    unsigned char* rgbImg =img2rgb(width,height,img);
    writePPM(outputPath,width,height,rgbImg);
    delete []img;
    free(rgbImg);
}

// Crée la scène : une Cornell Box avec une sphère
// Définit les murs avec différentes couleurs et ajoute une sphère rouge au centre
std::vector<Object*> createScene()
{
    // Définition des matériaux pour la Cornell Box
    Lambertian* red_wall = new Lambertian(Vec3(0.65, 0.05, 0.05));    // Rouge
    Lambertian* green_wall = new Lambertian(Vec3(0.12, 0.45, 0.15));  // Vert
    Lambertian* white_wall = new Lambertian(Vec3(0.73, 0.73, 0.73));  // Blanc
    Lambertian* red_material = new Lambertian(Vec3(0.8,0.3,0.3));

    std::vector<Object*> scene;
    
    // Mur du fond
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.0, -4.0), Vec3(2.0, 2.0, -3.99)));
    // Mur gauche (vert)
    scene.push_back(new AABB(green_wall, Vec3(-2.01, -2.0, -4.0), Vec3(-2.0, 2.0, 0.0)));
    // Mur droit (rouge)
    scene.push_back(new AABB(red_wall, Vec3(2.0, -2.0, -4.0), Vec3(2.01, 2.0, 0.0)));
    // Sol
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.01, -4.0), Vec3(2.0, -2.0, 0.0)));
    // Plafond
    scene.push_back(new AABB(white_wall, Vec3(-2.0, 2.0, -4.0), Vec3(2.0, 2.01, 0.0)));

    // Ajout d'une sphère rouge au centre
    scene.push_back(new Sphere(red_material, 0.5, Vec3(0.0, -1.5, -2.0)));

    return scene;
}