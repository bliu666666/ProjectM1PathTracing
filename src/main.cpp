#include <iostream>
#include "camera.h"
#include "viewport.h"
#include "scene.h"


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

    // Créer une scène test
    Scene scene;
    scene.addSphere(vec3(0, 0, 5), 1.0f);  // Sphère à 5 unités devant la caméra
    
    // Lancer un rayon
    Ray ray(cam.E, rayDir);
    HitInfo hit = scene.intersect(ray);
    
    if (hit.hit) {
        std::cout << "Intersection trouvée à la distance: " << hit.distance << std::endl;
        std::cout << "Point d'intersection: ";
        hit.point.print();
        std::cout << "Normale au point d'intersection: ";
        hit.normal.print();
    } else {
        std::cout << "Aucune intersection trouvée" << std::endl;
    }

    // Ajouter une boîte à la scène
    scene.addBox(vec3(-1, -1, 4), vec3(1, 1, 6));  // Boîte centrée en z=5
    
    // Test d'intersection avec la boîte
    Ray boxRay(cam.E, vec3(0, 0, 1).normalize());  // Rayon vers l'avant
    HitInfo boxHit = scene.intersect(boxRay);
    
    if (boxHit.hit) {
        std::cout << "Intersection avec la boîte trouvée à la distance: " << boxHit.distance << std::endl;
        std::cout << "Point d'intersection: ";
        boxHit.point.print();
        std::cout << "Normale au point d'intersection: ";
        boxHit.normal.print();
    }

    return 0;
}
