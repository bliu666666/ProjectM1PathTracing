#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "utils.h"
#include "../ppm_writer/ppm_writer.h"
#include <vector>

class Renderer {
private:
    unsigned width;
    unsigned height;
    unsigned samples_per_pixel;
    unsigned max_depth;

    std::vector<double> ray_color(const Ray& ray, const Scene& scene, int depth) const;

public:
    Renderer(unsigned width, unsigned height,
             unsigned samples = 100,
             unsigned depth = 50);

    void render(const Scene& scene, const char* outputPath) const;
};

#endif // RENDERER_H
