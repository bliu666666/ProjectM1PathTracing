#include "scene.h"

HitInfo Scene::intersect(const Ray& ray) const {
    HitInfo result = {false, INFINITY, vec3(), vec3()};
    
    for (const auto& sphere : spheres) {
        vec3 oc = ray.origin - sphere.center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - sphere.radius * sphere.radius;
        float discriminant = b*b - 4*a*c;
        
        if (discriminant > 0) {
            float temp = (-b - sqrt(discriminant)) / (2.0f*a);
            if (temp < result.distance && temp > 0.001f) {
                result.hit = true;
                result.distance = temp;
                result.point = ray.pointAtDistance(temp);
                result.normal = (result.point - sphere.center).normalize();
            }
        }
    }
    
    return result;
} 