#include <iostream>
#include "camera.h"
#include "viewport.h"


int main() {

    vec3 initialVec(1.0, 0.0, 0.0);
    camera cam;


    std::cout << "Vecteur initial: ";
    initialVec.print();


    float alpha = M_PI / 4; // 45 degrés
    float beta = M_PI / 4;  // 45 degrés
    
    vec3 pitch = cam.pitch(initialVec.getX(),initialVec.getY(),initialVec.getZ(),alpha);
    vec3 yaw = cam.yaw(initialVec.getX(),initialVec.getY(),initialVec.getZ(),beta);
    vec3 result = cam.compute(initialVec, alpha, beta);

    // Afficher le résultat
    std::cout << "Résultat pitch: ";
    pitch.print();

    std::cout << "Résultat yaw: ";
    yaw.print();

    std::cout << "Résultat après rotation (pitch + yaw): ";
    result.print();



    vec3 test = result + 1;
    test = test + result;
    test.print();

    viewport vp;
    vp.theta = M_PI / 2;  // 90 degrés de champ de vision
    vp.k = 800;           // largeur
    vp.m = 600;           // hauteur

    // Test pour un pixel spécifique
    vec3 rayDir = vp.computeRayVector(400, 300);  // pixel au centre approximativement
    std::cout << "Vecteur rayon pour le pixel (400,300): ";
    rayDir.print();

    return 0;
}
