#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "../ppm_writer/ppm_writer.h"

class Renderer {
private:
    unsigned width;
    unsigned height;

public:
    Renderer(unsigned width, unsigned height);

    void render(const Scene& scene, const char* outputPath) const;
};

#endif // RENDERER_H
