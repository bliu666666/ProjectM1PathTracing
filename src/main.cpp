#include <iostream>
#include "camera.h"
#include "viewport.h"
#include "scene.h"
#include "renderer.h"

int main() {
    Scene scene;

    scene.addSphere(vec3(0, 1, -4), 0.5f);      
    scene.addSphere(vec3(-2, 1, -6), 1.0f);     
    scene.addSphere(vec3(2, 0.5, -7), 0.7f);    
    scene.addSphere(vec3(-1, -0.6, -8), 1.2f);    
    scene.addSphere(vec3(1.5, -0.5, -11), 1.5f); 

    // Plafond
    scene.addBox(vec3(-4, -2.5, -10), vec3(4, -2, -3));  

    // Mur gauche
    scene.addBox(vec3(-4, -2.5, -10), vec3(-3.5, 2, -3));  

    // Mur droit
    scene.addBox(vec3(3.5, -2.5, -10), vec3(4, 2, -3));   

    // Configuration du viewport
    viewport vp;
    vp.theta = M_PI / 3;
    vp.k = 800;
    vp.m = 600;
    
    // Création et utilisation du renderer
    Renderer renderer(vp.k, vp.m);
    renderer.render(scene, vp);
    renderer.saveImage("output.ppm");
    
    std::cout << "Image générée avec succès dans output.ppm" << std::endl;
    
    return 0;
}
