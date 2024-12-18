#include "renderer.h"
#include <iostream>
#include <vector>

Renderer::Renderer(unsigned width, unsigned height) : width(width), height(height) {}

void Renderer::render(const Scene& scene, const char* outputPath) const {
    std::cout << "Début du rendu pour l'image : " << width << "x" << height << std::endl;
    std::vector<double> image(width * height * 3, 0.0);

    Camera camera = scene.getCamera();
    Position origin = camera.getPosition();

    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            double u = (2.0 * (x + 0.5) / width - 1.0) * std::tan(camera.getFov() * 0.5 * M_PI / 180.0);
            double v = (1.0 - 2.0 * (y + 0.5) / height) * std::tan(camera.getFov() * 0.5 * M_PI / 180.0);

            // Créer un rayon avec une direction normalisée
            Position direction(u, v, 1.0);
            direction.normalize(); 
            Ray ray(origin, direction);

            // Chercher l'intersection la plus proche
            Object* hitObject = nullptr;
            double t = 1e9; 
            if (scene.trace(ray, t, hitObject)) {
                // Récupérer la couleur de l'objet touché
                const std::vector<double>& color = hitObject->getColor();
                image[3 * (y * width + x) + 0] = color[0]; // Rouge
                image[3 * (y * width + x) + 1] = color[1]; // Vert
                image[3 * (y * width + x) + 2] = color[2]; // Bleu
            }
        }
    }

    std::cout << "On en est là 1" << std::endl;
    unsigned char* rgbImg = img2rgb(width, height, image.data());
    writePPM((char*)outputPath, width, height, rgbImg);
    free(rgbImg);
    std::cout << "On en est là 2" << std::endl;
}
