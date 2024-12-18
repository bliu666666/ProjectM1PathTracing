#include "scene.h"
#include <cmath>

HitInfo Scene::intersect(const Ray& ray) const {
    HitInfo result = {false, INFINITY, vec3(), vec3()};
    
    for (const auto& sphere : spheres) {
        vec3 oc = ray.origin - sphere.center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - sphere.radius * sphere.radius;
        float discriminant = b*b - 4*a*c;
        
        if (discriminant > 0) {
            float t1 = (-b - sqrt(discriminant)) / (2.0f*a);
            float t2 = (-b + sqrt(discriminant)) / (2.0f*a);
            
            float t = (t1 < t2 && t1 > 0.001f) ? t1 : t2;
            
            if (t > 0.001f && t < result.distance) {
                result.hit = true;
                result.distance = t;
                result.point = ray.pointAtDistance(t);
                result.normal = (result.point - sphere.center).normalize();
                
                if (ray.direction.dot(result.normal) > 0) {
                    result.normal = result.normal * -1;
                }
            }
        }
    }
    
    return result;
} 