#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>  // pour std::min et std::max
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
    
    struct Box {
        vec3 min;  // Point minimum (coin inférieur gauche)
        vec3 max;  // Point maximum (coin supérieur droit)
        Box(const vec3& min, const vec3& max) : min(min), max(max) {}
    };
    
    std::vector<Sphere> spheres;
    std::vector<Box> boxes;
    
    void addSphere(const vec3& center, float radius) {
        spheres.push_back(Sphere(center, radius));
    }
    
    void addBox(const vec3& min, const vec3& max) {
        boxes.push_back(Box(min, max));
    }
    
    HitInfo intersect(const Ray& ray) const;
    HitInfo intersectBox(const Ray& ray, const Box& box) const;
};

#endif 