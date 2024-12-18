#include "renderer.h"
#include <cmath>
#include <iostream>

void Renderer::render(const Scene& scene, const viewport& vp) {
    std::cout << "Début du rendu..." << std::endl;
    
    // Initialiser l'image avec une couleur de test
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = 3 * (j * width + i);
            image[index] = 0.0;     // R
            image[index + 1] = 0.0; // G
            image[index + 2] = 0.0; // B
        }
    }

    // Pour chaque pixel de l'image
    for (int j = 0; j < height; j++) {
        if (j % 100 == 0) {
            std::cout << "Progression: " << (100.0f * j / height) << "%" << std::endl;
        }
        
        for (int i = 0; i < width; i++) {
            // Calculer le rayon pour ce pixel
            vec3 rayDir = vp.computeRayVector(i, j);
            Ray ray(vp.cam.E, rayDir);
            
            // Calculer la couleur pour ce rayon
            vec3 color = computeColor(ray, scene);
            
            // Stocker la couleur dans l'image
            int index = 3 * (j * width + i);

            image[index] = std::max(0.0f, std::min(1.0f, color.getX()));     // R
            image[index + 1] = std::max(0.0f, std::min(1.0f, color.getY())); // G
            image[index + 2] = std::max(0.0f, std::min(1.0f, color.getZ())); // B
        }
    }
    
    std::cout << "Rendu terminé." << std::endl;
}

vec3 Renderer::computeColor(const Ray& ray, const Scene& scene, int depth) {
    if (depth > 3) {
        return vec3(0, 0, 0);
    }

    HitInfo hit = scene.intersect(ray);
    
    if (hit.hit) {
        // Calculer la position relative de l'objet pour déterminer sa couleur
        vec3 hitPoint = ray.pointAtDistance(hit.distance);
        
        // C'est oite
        if (std::abs(hit.normal.getX()) == 1 || 
            std::abs(hit.normal.getY()) == 1 || 
            std::abs(hit.normal.getZ()) == 1) {
            return vec3(0.8, 0.8, 0.0);  // Jaune pour la boîte
        }
        
        // Pour les sphères, couleur basée sur leur position
        if (hitPoint.getZ() < -10) {
            return vec3(0.0, 0.0, 0.0);  // Noir
        } else {
            
            return vec3((10-abs(hitPoint.getZ()))/10, 0.0, 0.0);  // Rouge
        }

    }
    
    // Couleur du fond
    return vec3(0.2, 0.3, 1.0);  // Bleu
}

void Renderer::saveImage(const char* filename) {
    std::cout << "Sauvegarde de l'image dans " << filename << std::endl;
    
    
    unsigned char* rgbImage = img2rgb(width, height, image);
    
    writePPM((char*)filename, width, height, rgbImage);
    
    free(rgbImage);
    
    std::cout << "Image sauvegardée." << std::endl;
} 