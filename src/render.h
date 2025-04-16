#pragma once

#include "firstCollision.h"
#include "camera.h"
#include "../ppm_writer/ppm_writer.c"
#include <omp.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "lambertian.h"
#include "perfectDiffuse.h"
#include "refractive.h"
#include "emissive.h"           // Pour les matériaux émissifs (néon)
#include "environment.h"        // Pour le sky lighting
#include "ambientOcclusion.h"   // Pour l'occlusion ambiante
#include <chrono>
#include <algorithm> // Pour std::clamp et autres fonctions algorithmiques

// Variable globale pour l'environnement (sky lighting)
GradientSky* environment = nullptr;


// Fonction pour limiter les valeurs extrêmes (réduction des fireflies)
// Fonction améliorée pour limiter les valeurs extrêmes avec une transition plus douce
inline Vec3 clampColor(const Vec3& color, double max_value = 1.0) {
    // Réduction des "fireflies" avec une fonction de mappage Reinhard modifiée
    Vec3 result;
    result.x = color.x / (1.0 + color.x);
    result.y = color.y / (1.0 + color.y);
    result.z = color.z / (1.0 + color.z);
    
    // Puis application d'un clamp traditionnel pour s'assurer que rien ne dépasse max_value
    return Vec3(
        std::min(result.x, max_value),
        std::min(result.y, max_value),
        std::min(result.z, max_value)
    );
}

// Calcule la couleur d'un point en fonction des intersections du rayon
// Si le rayon touche un objet, calcule la couleur basée sur le matériau
// Sinon, retourne la couleur de l'environnement
Vec3 calculateColor(const Ray& ray, const std::vector<Object*>& scene, int depth)
{
    if (depth <= 0) {
        // Utiliser une valeur minimale non nulle pour améliorer la convergence
        return Vec3(0.02,0.02,0.02); // Valeur minimale plutôt que noir complet
    }

    HitInfo hitInfo = findFirstCollision(ray,scene);
    
    // Si le rayon ne touche aucun objet, retourner la couleur de l'environnement
    if (!hitInfo.hitObject) {
        if (environment) {
            return environment->sample(ray.direction);
        } else {
            // Couleur de fond en dégradé (si pas d'environnement)
            Vec3 unitDirection = ray.direction.normalize();
            double t = (unitDirection.y + 1.0) / 2;
            return (1.0-t) * Vec3(0.8,0.8,0.8) + t * Vec3(0.4,0.5,0.8);
        }
    }
    
    // Vérifier si le matériau est émissif
    Vec3 emitted = hitInfo.hitObject->material->emitted();
    if (emitted.length() > 0) {
        return emitted;  // Si c'est un matériau émissif, retourner directement sa couleur
    }
    
    Ray scattered;
    Vec3 attenuation;
    
    // Si le matériau diffuse la lumière
    if (hitInfo.hitObject->material->scatter(ray,hitInfo,attenuation,scattered)) {
            // Calcul de l'illumination directe (échantillonnage de la lumière)
            Vec3 direct_light(0,0,0);
            
            // Échantillonnage stratégique multiple de la source lumineuse (réduction du bruit)
            // Utilisation de plusieurs échantillons par point d'intersection
            Vec3 light_sum(0,0,0);
            int light_samples = 5; // Nombre d'échantillons de lumière
            
            for (int ls = 0; ls < light_samples; ls++) {
                // Stratégie d'échantillonnage stratifié pour une meilleure couverture
                double u = (ls + randomDouble()) / light_samples;
                double v = randomDouble();
                
                // Mapping sur la surface de la lumière
                double light_x = -0.5 + u;
                double light_z = -2.0 + v;
                Vec3 light_pos = Vec3(light_x, 1.99, light_z);
                Vec3 to_light = (light_pos - hitInfo.hitPoint).normalize();
                double light_distance = (light_pos - hitInfo.hitPoint).length();
            
                // Vérification de la visibilité de la lumière (ombres douces)
                Ray shadow_ray(hitInfo.hitPoint + hitInfo.normal * 0.001, to_light);
                HitInfo shadow_info = findFirstCollision(shadow_ray, scene);
                
                // Évaluation de la contribution de cet échantillon
                if (!shadow_info.hitObject || shadow_info.t > light_distance)
                {
                    double cos_theta = dot(hitInfo.normal, to_light);
                    if (cos_theta > 0)
                    {
                        // Facteur d'atténuation physique selon la distance
                        double falloff = 1.0 / (1.0 + 0.1 * light_distance * light_distance);
                        
                        // Intensité de lumière ajustée avec falloff
                        double light_intensity = 30.0 * falloff;
                        light_sum = light_sum + attenuation * cos_theta * Vec3(light_intensity, light_intensity, light_intensity);
                    }
                }
            }
            
            // Moyenne des échantillons de lumière
            direct_light = light_sum / light_samples;
            
            // Calcul de l'illumination indirecte (rebonds)
            Vec3 indirect_light = attenuation * calculateColor(scattered, scene, depth - 1);
            
            // Calculer l'occlusion ambiante
            double ao_factor = calculateAmbientOcclusion(hitInfo.hitPoint, hitInfo.normal, scene);
            
            // Lumière ambiante basée sur l'occlusion
            Vec3 ambient_light = attenuation * Vec3(0.1, 0.1, 0.15) * ao_factor;
            
            // Combiner l'éclairage direct, indirect et ambiant
            Vec3 result;
            if (dynamic_cast<Refractive*>(hitInfo.hitObject->material)) {
                result = direct_light * 0.3 + indirect_light * 0.65 + ambient_light * 0.05; // Verre
            } else if (dynamic_cast<Lambertian*>(hitInfo.hitObject->material)) {
                result = direct_light * 0.4 + indirect_light * 0.5 + ambient_light * 0.1; // Lambertien
            } else {
                // PerfectDiffuse
                result = direct_light * 0.3 + indirect_light * 0.6 + ambient_light * 0.1; // Plus de color bleeding
            }
            
            // Limiter strictement les valeurs pour éliminer les fireflies
            return clampColor(result);
    }
    
    // Si le matériau ne diffuse pas, retourne noir
    return Vec3(0,0,0);
}

// Fonction principale de rendu
// Parcourt chaque pixel et calcule sa couleur par échantillonnage multiple

void render(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
            const Vec3& v_up, double v_fov,int samples_per_pixel,int max_depth)
{
    // Chronométrage précis pour chaque étape du rendu
    auto start_total = std::chrono::high_resolution_clock::now();
    
    // Initialiser l'environnement (sky lighting)
    if (!environment) {
        environment = new GradientSky(
            Vec3(0.2, 0.4, 0.8),    // Bleu ciel au zénith
            Vec3(0.8, 0.8, 0.9)     // Blanc bleuâtre à l'horizon
        );
    }
    
    // Afficher les informations sur le rendu et la parallélisation
    int max_threads = omp_get_max_threads();
    std::cout << "\nDébut du rendu avec " << max_threads << " threads" << std::endl;
    std::cout << "Résolution: " << width << "x" << height << ", Échantillons: " << samples_per_pixel << ", Profondeur max: " << max_depth << std::endl;

    double aspect=width/height;
    
    // Configuration de la profondeur de champ (DOF)
    double aperture = 0.05;      // Taille de l'ouverture (plus la valeur est grande, plus le flou est important)
    double focus_dist = 5.0;     // Distance de mise au point
    
    // Utilise les paramètres spécifiés par l'utilisateur pour la caméra avec profondeur de champ
    Camera camera(origin, lookat, v_up, v_fov, aspect, aperture, focus_dist);
    double *img=new double[static_cast<int>(width)*static_cast<int>(height)*3];
    
    // Initialisation de l'image avec des valeurs nulles
    #pragma omp parallel for simd
    for (int i = 0; i < static_cast<int>(width) * static_cast<int>(height) * 3; i++) {
        img[i] = 0.0;
    }
    
    int total_pixels = static_cast<int>(width) * static_cast<int>(height);
    int processed_pixels = 0;
    int update_interval = total_pixels / 20; // Mise à jour tous les 5%
    
    // Parcours optimisé des pixels avec meilleure parallélisation
    auto start_render = std::chrono::high_resolution_clock::now();
    
    // Subdivision des tâches pour une meilleure répartition de la charge
    #pragma omp parallel for schedule(dynamic, 16) collapse(2)
    for (int i=height-1;i>=0;--i)
    {
        for (int j=0;j<width;++j)
        {
            // Seed différente pour chaque thread pour éviter les corrélations
            int thread_id = omp_get_thread_num();
            // Utilisation du thread_id et des coordonnées du pixel pour une meilleure diversité de seeds
            unsigned int seed = static_cast<unsigned int>(thread_id * 1000 + i * width + j);
            std::mt19937 gen(seed);
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            // Fonction lambda pour générer des nombres aléatoires thread-safe
            auto thread_rand = [&]() -> double {
                return dist(gen);
            };
            
            Vec3 color(0,0,0);
            // Échantillonnage stratifié pour une meilleure convergence et réduction du bruit
            for (int s=0;s<samples_per_pixel;++s) {
                // Échantillonnage stratifié amélioré avec distribution de Halton
                // pour une meilleure distribution des échantillons
                int sqrt_spp = sqrt(samples_per_pixel);
                int s_i = s / sqrt_spp;
                int s_j = s % sqrt_spp;
                
                // Utilisation d'une meilleure distribution aléatoire par thread
                double jitter_x = thread_rand(); 
                double jitter_y = thread_rand();
                
                // Position dans le sous-pixel avec jitter (bruit aléatoire)
                double u = (j + (s_j + jitter_x) / sqrt_spp) / width;
                double v = (i + (s_i + jitter_y) / sqrt_spp) / height;
                
                // Générer un rayon avec la caméra améliorée (la profondeur de champ est gérée par la caméra)
                Ray ray = camera.getRay(u, v);
                color = color + calculateColor(ray, scene, max_depth);
            }
            color=color/static_cast<double>(samples_per_pixel);// Moyenne des échantillons
            
            #pragma omp atomic
            processed_pixels++;
            
            // Affichage de l'avancement toutes les 5%
            if (omp_get_thread_num() == 0 && processed_pixels % update_interval == 0) {
                double progress = 100.0 * processed_pixels / total_pixels;
                std::cout << "\rProgression: " << std::fixed << std::setprecision(1) << progress << "% " << std::flush;
            }

            // Conversion des couleurs dans l'intervalle [0,255]
            // Correction gamma plus précise avec ACES tonemapping pour améliorer la plage dynamique
            // ACES filmic tone mapping approximation (améliorée)
            double a = 2.51;
            double b = 0.03;
            double c = 2.43;
            double d = 0.59;
            double e = 0.14;
            
            // Appliquer le tone mapping à chaque composante
            color.x = (color.x * (a * color.x + b)) / (color.x * (c * color.x + d) + e);
            color.y = (color.y * (a * color.y + b)) / (color.y * (c * color.y + d) + e);
            color.z = (color.z * (a * color.z + b)) / (color.z * (c * color.z + d) + e);
            
            // Correction gamma standard (gamma = 2.2 au lieu de 2.0)
            double gamma = 2.2;
            color.x = pow(std::clamp(color.x, 0.0, 1.0), 1.0 / gamma);
            color.y = pow(std::clamp(color.y, 0.0, 1.0), 1.0 / gamma);
            color.z = pow(std::clamp(color.z, 0.0, 1.0), 1.0 / gamma);
            
            img[3*(i*static_cast<int>(width)+j)+0]=color.x;
            img[3*(i*static_cast<int>(width)+j)+1]=color.y;
            img[3*(i*static_cast<int>(width)+j)+2]=color.z;
        }
    }
    auto end_render = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> render_time = end_render - start_render;
    std::cout << "\nRendu terminé en " << render_time.count() << " secondes" << std::endl;
    
    // Début de la conversion et écriture
    auto start_write = std::chrono::high_resolution_clock::now();
    std::cout << "Conversion et écriture de l'image..." << std::endl;
    
    unsigned char* rgbImg = img2rgb(width,height,img);
    writePPM(outputPath,width,height,rgbImg);
    
    auto end_write = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> write_time = end_write - start_write;
    std::cout << "Image écrite en " << write_time.count() << " secondes" << std::endl;
    
    delete []img;
    free(rgbImg);
    
    // Statistiques finales
    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = end_total - start_total;
    std::cout << "Temps total: " << total_time.count() << " secondes" << std::endl;
    std::cout << "Performance: " << (width * height * samples_per_pixel) / total_time.count() / 1000000 << " millions d'échantillons/seconde" << std::endl;
}

// Crée la scène : une Cornell Box avec une sphère
// Définit les murs avec différentes couleurs et ajoute une sphère rouge au centre
std::vector<Object*> createScene()
{
    // Définition des matériaux pour la Cornell Box
    // Matériaux diffus
    PerfectDiffuse* red_wall = new PerfectDiffuse(Vec3(0.95, 0.05, 0.05));    // Rouge
    PerfectDiffuse* green_wall = new PerfectDiffuse(Vec3(0.12, 0.95, 0.15));  // Vert
    Lambertian* white_wall = new Lambertian(Vec3(0.5, 0.5, 0.5));  // Blanc
    
    // Matériaux spéciaux
    Refractive* glass = new Refractive(1.5);  // Verre (indice de réfraction = 1.5)
    Lambertian* red_material = new Lambertian(Vec3(0.8, 0.3, 0.3));
    
    // Matériaux émissifs (pour les effets néon)
    Emissive* light_source = new Emissive(Vec3(20.0, 20.0, 20.0));    // Source lumineuse principale plus puissante (blanc)
    Emissive* blue_neon = new Emissive(Vec3(0.0, 2.0, 8.0));          // Néon bleu beaucoup plus intense
    Emissive* orange_neon = new Emissive(Vec3(8.0, 1.5, 0.0));        // Néon orange plus vif
    
    std::vector<Object*> scene;
    
    // Murs de la Cornell Box
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.0, -4.0), Vec3(2.0, 2.0, -3.99)));       // Mur du fond
    scene.push_back(new AABB(green_wall, Vec3(-2.01, -2.0, -4.0), Vec3(-2.0, 2.0, 0.0)));         // Mur gauche
    scene.push_back(new AABB(red_wall, Vec3(2.0, -2.0, -4.0), Vec3(2.01, 2.0, 0.0)));         // Mur droit
    scene.push_back(new AABB(white_wall, Vec3(2.0, -2.0, -4.0), Vec3(2.0, -2.0, 0.0)));       // Sol
    scene.push_back(new AABB(white_wall, Vec3(-2.0, 2.0, -4.0), Vec3(2.0, 2.01, 0.0)));         // Plafond
    
    // Ajout d'une sphère rouge au centre
    scene.push_back(new Sphere(red_material, 0.5, Vec3(0.0, -1.5, -2.0)));
    // Source de lumière au plafond
    scene.push_back(new AABB(light_source, Vec3(-0.5, 1.9, -2), Vec3(0.5, 2, -1)));      // Plafond lumineux plus petit et centré
    
    // Sphère en verre pour les caustiques
    scene.push_back(new Sphere(glass, 0.7, Vec3(0.5, -1.0, -2.0)));
    
    // Petite sphère diffuse pour montrer les ombres et le color bleeding
    scene.push_back(new Sphere(white_wall, 0.5, Vec3(-0.7, -1.2, -1.5)));
    
    // Ajout des éléments néon
    scene.push_back(new AABB(blue_neon, Vec3(-1.8, -1.0, -2.5), Vec3(-1.6, 0.5, -2.3)));    // Tube néon bleu vertical
    scene.push_back(new AABB(orange_neon, Vec3(1.5, -0.7, -3.0), Vec3(1.8, -0.5, -1.5)));  // Tube néon orange horizontal
    
    return scene;
}