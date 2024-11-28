#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <random>
 
#include "ray.h"
#include "color.h"
#include "viewport.h"
#include "sphere.h" 
#include "hitting.h"

class PathTracer {
public:
    int width, height;
    int samples_per_pixel = 100;
    std::vector<Sphere> spheres;

    PathTracer(int w, int h) : width(w), height(h) {
        spheres.emplace_back(vec3(0, 0, -5), 2, vec3(1, 1, 0));
    }

    double random_double() {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    void render(viewport &vp, float aspect_ratio, const char *path) {
        std::ofstream file(path);
        file << "P3\n" << width << " " << height << "\n255\n";

        for (int j = height - 1; j >= 0; --j) {
            for (int i = 0; i < width; ++i) {
                vec3 pixel_color(0, 0, 0);
                
                for (int s = 0; s < samples_per_pixel; ++s) {
                    float u = (2.0f * (float(i) + random_double()) / float(width) - 1.0f) * aspect_ratio;
                    float v = 2.0f * (float(j) + random_double()) / float(height) - 1.0f;
                    
                    ray r(vp.get_origin(), vec3(u, v, -1).normalize());
                    pixel_color = pixel_color + Hitting::trace(r, spheres);
                }

                pixel_color = pixel_color * (1.0 / samples_per_pixel);

                int r = static_cast<int>(256 * std::clamp(pixel_color.x, 0.0, 0.999));
                int g = static_cast<int>(256 * std::clamp(pixel_color.y, 0.0, 0.999));
                int b = static_cast<int>(256 * std::clamp(pixel_color.z, 0.0, 0.999));
                
                file << r << " " << g << " " << b << "\n";
            }
        }
        file.close();
    }
};
#endif // RENDER_H
