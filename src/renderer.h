#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "viewport.h"
#include "camera.h"
#include "../ppm_writer/ppm_writer.h"

class Renderer {
public:
    Renderer(int width, int height) : width(width), height(height) {
        image = new double[width * height * 3];
    }
    
    ~Renderer() {
        delete[] image;
    }

    void render(const Scene& scene, const viewport& vp);
    void saveImage(const char* filename);

private:
    int width;
    int height;
    double* image;
    
    vec3 computeColor(const Ray& ray, const Scene& scene, int depth = 0);
};

#endif 