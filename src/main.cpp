#include "scene.h"
#include "../shapes/sphere.h"
#include "../shapes/cube.h"
#include "renderer.h"
#include <iostream>

int main() {
    std::cout << "Début du main" << std::endl;
    // Dimensions de l'image
    const unsigned width = 800;
    const unsigned height = 600;
    const char* outputFile = "../images/output.ppm";

    // Création de la caméra
    Camera camera(Position(0.0, 0.0, -5.0), Position(0.0, 0.0, 1.0), 90.0, width, height);

    // Créer une scène
    Scene scene;
    scene.setCamera(camera);

    // Ajouts des objets à la scène
    // Ajouter une sphère rouge
    Sphere* sphere1 = new Sphere(Position(0.0, 0.0, 5.0), 1.0, {1.0, 0.0, 0.0});
    scene.addObject(sphere1);

    // Ajouter une sphère verte
    Sphere* sphere2 = new Sphere(Position(2.0, 0.0, 6.0), 0.5,{0.0, 1.0, 0.0});
    scene.addObject(sphere2);  

    // Ajouter un cube vert
    Cube* cube1 = new Cube(Position(5.0, 0.0, 6.0), 1.0, {0.0, 2.0, 0.0});
    scene.addObject(cube1);

    // Configurer le renderer
    Renderer renderer(width, height);
    std::cout << "On lance le rendu" << std::endl;

    // Lancer le rendu
    renderer.render(scene, outputFile);
    std::cout << "Rendu fini" << std::endl;

    // Nettoyer la mémoire allouée dynamiquement
    delete sphere1;
    delete sphere2;
    delete cube1;
    std::cout << "Fin du main" << std::endl;
    return 0;
}
