#ifndef RAY_COLOR_H
#define RAY_COLOR_H

#include "ray.h"
#include <strstream>    

color ray_color(const ray& r) {
    vec3 unit_direction =r.get_direction().normalize();
    auto a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

#endif // !RAY_COLOR_H


