#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
    vec3 origin;     // Point d'origine du rayon
    vec3 direction;  // Direction du rayon
    
    Ray(const vec3& origin, const vec3& direction) 
        : origin(origin), direction(direction.normalize()) {}
    
    vec3 pointAtDistance(float t) const {
        return origin + direction * t;
    }
};

#endif 