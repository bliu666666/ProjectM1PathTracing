#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <string>
#include "viewport.h"
#include "vec3.h"
#include "ray.h"

class RayTracer {
private:
    int image_width;
    int image_height;
    double random_double();
    
public:
    RayTracer(int width, int height);
    void render(const Viewport& vp, float aspect_ratio, const std::string& filename, bool anti_aliasing);
};

#endif
