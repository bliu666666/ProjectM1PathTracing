#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"

class Sphere {
public:
    vec3 center;
    float radius;
    vec3 color;

    Sphere(const vec3& c, float r, const vec3& col) : center(c), radius(r), color(col) {}

    bool hit(const ray& ray, float& t) const {
        vec3 oc = ray.get_origin() - center;
        float a = ray.get_direction().dot(ray.get_direction());
        float b = 2.0 * oc.dot(ray.get_direction());
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            t = (-b - sqrt(discriminant)) / (2.0f * a);
            return t > 0;
        }
        return false;
    }
};

#endif 
