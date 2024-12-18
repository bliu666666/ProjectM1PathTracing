#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "ray.h"

struct HitInfo {
    bool hit;           // Si il y a eu intersection
    float distance;     // Distance de l'intersection
    vec3 point;        // Point d'intersection
    vec3 normal;       // Normale au point d'intersection
};

class Scene {
public:
    struct Sphere {
        vec3 center;
        float radius;
        Sphere(const vec3& c, float r) : center(c), radius(r) {}
    };
    
    std::vector<Sphere> spheres;
    
    void addSphere(const vec3& center, float radius) {
        spheres.push_back(Sphere(center, radius));
    }
    
    HitInfo intersect(const Ray& ray) const;
};

#endif 