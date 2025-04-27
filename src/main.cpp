#include "scene.h"
#include "../shapes/sphere.h"
#include "../shapes/AABB.h"
#include "renderer.h"
#include "Lambertian.h"
#include "Emissive.h"
#include "Metallic.h"
#include "Dielectric.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <unistd.h>  // Pour getopt
#include <omp.h>     // Pour omp_set_num_threads
#include <filesystem> // Pour std::filesystem

void print_usage() {
    std::cout << "Usage: RayTracer [options]\n"
              << "Options:\n"
              << "  -w <width>      Largeur de l'image (défaut: 800)\n"
              << "  -h <height>     Hauteur de l'image (défaut: 600)\n"
              << "  -s <samples>    Nombre d'échantillons par pixel (défaut: 100)\n"
              << "  -d <depth>      Profondeur maximale de récursion (défaut: 50)\n"
              << "  -t <threads>    Nombre de threads OpenMP (défaut: nombre de cœurs disponibles)\n"
              << "  -o <file>       Fichier de sortie (défaut: images/output.ppm)\n";
}

void cornell_box(Scene& scene) {
    // Création des matériaux
    auto matWhite = std::make_shared<Lambertian>(std::vector<double>{0.73, 0.73, 0.73});
    auto matRed = std::make_shared<Lambertian>(std::vector<double>{0.65, 0.05, 0.05});
    auto matGreen = std::make_shared<Lambertian>(std::vector<double>{0.12, 0.45, 0.15});
    auto matLight = std::make_shared<Emissive>(std::vector<double>{5.0, 5.0, 5.0});

    // Dimensions de la Cornell Box
    const double box_size = 5.0;
    const double wall_thickness = 0.1;
    const double z_offset = 3.0;  // Décalage en Z pour éloigner les objets

    const double light_size = box_size * 0.3; // Taille de la zone lumineuse

    // Ajout des murs de la Cornell Box
    // Mur arrière (blanc)
    AABB* back_wall = new AABB(Position(0.0, 0.0, box_size/2 + z_offset), box_size, box_size, wall_thickness, matWhite);
    scene.addObject(back_wall);

    // Mur gauche (rouge)
    AABB* left_wall = new AABB(Position(-box_size/2, 0.0, z_offset), wall_thickness, box_size, box_size, matRed);
    scene.addObject(left_wall);

    // Mur droit (vert)
    AABB* right_wall = new AABB(Position(box_size/2, 0.0, z_offset), wall_thickness, box_size, box_size, matGreen);
    scene.addObject(right_wall);

    // Plafond (blanc)
    AABB* ceiling = new AABB(Position(0.0, box_size/2, z_offset), box_size, wall_thickness, box_size, matWhite);
    scene.addObject(ceiling);

    // Sol (blanc)
    AABB* floor = new AABB(Position(0.0, -box_size/2, z_offset), box_size, wall_thickness, box_size, matWhite);
    scene.addObject(floor);

    // Source de lumière au plafond
    AABB* light = new AABB(
        Position(0.0, box_size/2 - wall_thickness/2, z_offset),
        light_size, wall_thickness, light_size,
        matLight
    );
    scene.addObject(light);

}

int main(int argc, char* argv[]) {
    std::cout << "Début du main" << std::endl;
    
    // Valeurs par défaut
    unsigned width = 800;
    unsigned height = 600;
    unsigned samples = 100;
    unsigned depth = 50;
    int threads = omp_get_max_threads();
    
    // Obtenir le chemin absolu vers le dossier images
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path project_root = current_path.parent_path();
    std::filesystem::path default_output = project_root / "images" / "output.ppm";
    const char* outputFile = default_output.c_str();

    // Lecture des arguments
    int opt;
    while ((opt = getopt(argc, argv, "w:h:s:d:t:o:")) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 's':
                samples = atoi(optarg);
                break;
            case 'd':
                depth = atoi(optarg);
                break;
            case 't':
                threads = atoi(optarg);
                break;
            case 'o':
                outputFile = optarg;
                break;
            default:
                print_usage();
                return 1;
        }
    }

    // Configurer le nombre de threads OpenMP
    omp_set_num_threads(threads);
    std::cout << "Utilisation de " << threads << " threads" << std::endl;

    const double aspect_ratio = (double)width / height;

    // Paramètres de la caméra pour la Cornell Box
    Position lookfrom(0.0, 0.0, -5.0);  // Position de la caméra
    Position lookat(0.0, 0.0, 0.0);     // Point visé (vers l'origine de la scène)
    Position vup(0.0, 1.0, 0.0);        // Vecteur "up" (Y est le haut)
    double vfov = 40.0;                 // Champ de vision vertical réduit pour mieux voir la box

    // Création de la caméra
    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Créer une scène en lui passant la caméra
    Scene scene(camera);

    cornell_box(scene);
    
    // Nouveaux matériaux métalliques
    auto Gold = std::make_shared<Metallic>(std::vector<double>{0.8, 0.6, 0.2}, 0.7); // Or légèrement flou
    auto Mirror = std::make_shared<Metallic>(std::vector<double>{0.7, 0.7, 0.7}, 0.0); // Miroir parfait
    auto RoughMetal = std::make_shared<Metallic>(std::vector<double>{0.4, 0.4, 0.4}, 0.3); // Métal rugueux
    auto Blue = std::make_shared<Lambertian>(std::vector<double>{0.1, 0.1, 0.9});
    auto Orange = std::make_shared<Lambertian>(std::vector<double>{0.9, 0.4, 0.1});
    auto Glass = std::make_shared<Dielectric>(1.6);
    
    //AABB* orange_box = new AABB(Position(-1, -1, 2), 1, 1, 1, Orange);
    //scene.addObject(orange_box);

    //AABB* mirror_box = new AABB(Position(1, -1, 2), 1, 1, 1, Mirror);
    //scene.addObject(mirror_box);

    Sphere* glass_sphere = new Sphere(Position(0, 1, 3.5), 0.5, Glass);
    scene.addObject(glass_sphere);

    Sphere* blue_sphere = new Sphere(Position(1, 0, 3.5), 0.5, Blue);
    scene.addObject(blue_sphere);
    // Ajout des objets métalliques
    // Sphère miroir
    Sphere* gold_sphere = new Sphere(Position(0, -1, 3.5), 0.5, Gold);
    scene.addObject(gold_sphere);

    // Sphère métallique rugueuse
    Sphere* rough_sphere = new Sphere(Position(-1, 0, 3.5), 0.5, RoughMetal);
    scene.addObject(rough_sphere);


    

    // Configurer le renderer
    Renderer renderer(width, height, samples, depth);
    std::cout << "On lance le rendu" << std::endl;

    // Lancer le rendu
    renderer.render(scene, outputFile);
    std::cout << "Rendu fini" << std::endl;

    // La Scene s'occupe de libérer la mémoire des objets (et matériaux via shared_ptr)
    std::cout << "Fin du main" << std::endl;
    return 0;
}
