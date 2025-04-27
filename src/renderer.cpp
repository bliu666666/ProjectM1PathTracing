#include "renderer.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "material.h"
#include <omp.h>
#include <atomic>

// Constructeur
Renderer::Renderer(unsigned width, unsigned height, unsigned samples, unsigned depth)
    : width(width), height(height),
      samples_per_pixel(samples), max_depth(depth) {}

// Calcule la couleur pour un rayon donné
std::vector<double> Renderer::ray_color(const Ray& ray, const Scene& scene, int depth) const {
    if (depth <= 0) {
        return {0.0, 0.0, 0.0};
    }

    HitRecord rec;
    if (scene.trace(ray, rec)) {
        std::vector<double> emitted = rec.material->emitted(0, 0, rec.point);

        Ray scattered;
        std::vector<double> attenuation;

        if (rec.material->scatter(ray, rec, attenuation, scattered)) {
            std::vector<double> scattered_color = ray_color(scattered, scene, depth - 1);
            return {
                emitted[0] + attenuation[0] * scattered_color[0],
                emitted[1] + attenuation[1] * scattered_color[1],
                emitted[2] + attenuation[2] * scattered_color[2]
            };
        }

        return emitted;
    }

    return {0.0, 0.0, 0.0};
}

void Renderer::render(const Scene& scene, const char* outputPath) const {
    std::cout << "Début du rendu : " << width << "x" << height
              << " (" << samples_per_pixel << " échantillons par pixel)" << std::endl;

    std::vector<double> image(width * height * 3);
    Camera camera = scene.getCamera();

    int num_threads = omp_get_max_threads();
    std::cout << "Utilisation de " << num_threads << " threads" << std::endl;

    std::atomic<unsigned int> pixels_done = 0;
    const unsigned int total_pixels = width * height;

    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            std::vector<double> local_pixel_color = {0.0, 0.0, 0.0};

            for (unsigned s = 0; s < samples_per_pixel; s++) {
                double u_norm = (double)(x + random_double()) / width;
                double v_norm = (double)(height - 1 - y + random_double()) / height;

                Ray ray = camera.get_ray(u_norm, v_norm);

                std::vector<double> sample_color = ray_color(ray, scene, max_depth);
                local_pixel_color[0] += sample_color[0];
                local_pixel_color[1] += sample_color[1];
                local_pixel_color[2] += sample_color[2];
            }

            double scale = 1.0 / samples_per_pixel;
            double gamma = 2.0;
            local_pixel_color[0] = std::pow(local_pixel_color[0] * scale, 1.0/gamma);
            local_pixel_color[1] = std::pow(local_pixel_color[1] * scale, 1.0/gamma);
            local_pixel_color[2] = std::pow(local_pixel_color[2] * scale, 1.0/gamma);

            local_pixel_color[0] = std::min(1.0, std::max(0.0, local_pixel_color[0]));
            local_pixel_color[1] = std::min(1.0, std::max(0.0, local_pixel_color[1]));
            local_pixel_color[2] = std::min(1.0, std::max(0.0, local_pixel_color[2]));

            image[3 * (y * width + x) + 0] = local_pixel_color[0];
            image[3 * (y * width + x) + 1] = local_pixel_color[1];
            image[3 * (y * width + x) + 2] = local_pixel_color[2];

            unsigned int current_pixels = ++pixels_done;

            if (current_pixels % (total_pixels / 10) == 0 || current_pixels == total_pixels) {
                #pragma omp critical
                {
                    std::cout << "\rProgression: " << (current_pixels * 100) / total_pixels << "% (" 
                              << current_pixels << "/" << total_pixels << ")" << std::flush;
                }
            }
        }
    }

    std::cout << "\nConversion en RGB et écriture du fichier PPM..." << std::endl;
    unsigned char* rgbImg = img2rgb(width, height, image.data());
    writePPM((char*)outputPath, width, height, rgbImg);
    free(rgbImg);
    std::cout << "Fichés PPM écrit." << std::endl;
}
