#pragma once

#include "ray.h"
#include "vec3.h"

class Viewport {
public:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    Viewport() {}
    Viewport(const Vec3& origin, const Vec3& lower_left, const Vec3& horizontal, const Vec3& vertical)
        : origin(origin), lower_left_corner(lower_left), horizontal(horizontal), vertical(vertical) {}
    
    Ray get_ray(double u, double v) const {
        return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }
};
