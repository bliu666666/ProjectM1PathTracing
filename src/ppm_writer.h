#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
 
#include "ray.h"
#include "color.h"
#include "viewport.h"
#include "sphere.h" 
#include "hitting.h"

class PathTracer {
public:
    int width, height;
    std::vector<Sphere> spheres;

    PathTracer(int w, int h) : width(w), height(h) {
        spheres.emplace_back(vec3(0, 0, -5), 2, vec3(0.7, 0.3, 0.3)); // Red sphere
    }
    void render(viewport &vp,float aspect_ratio,const char *path) {
        std::ofstream file(path);
        file << "P3\n" << width << " " << height << "\n255\n";

        for (int j = height - 1; j >= 0; --j) {
            for (int i = 0; i < width; ++i) {
              // normalization depending on heigh image
              float u = (2.0f * float(i) / float(width) - 1.0f) * aspect_ratio;
              float v = 2.0f * float(j) / float(height) - 1.0f;
              ray ray(vp.get_origin(), vec3(u, v, -1).normalize());  
              vec3 color = Hitting::trace(ray, spheres);
              int r = static_cast<int>(255.99 * color.x);
              int g = static_cast<int>(255.99 * color.y);
              int b = static_cast<int>(255.99 * color.z);
              file << r << " " << g << " " << b << "\n";
            }
        }
        file.close();
    }
};
#endif // RENDER_H
