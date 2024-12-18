#include "scene.h"
#include <cmath>

HitInfo Scene::intersectBox(const Ray& ray, const Box& box) const {
    HitInfo result = {false, INFINITY, vec3(), vec3()};
    

    
    // Calcul des intersections pour chaque paire de plans
    float tmin = -INFINITY;
    float tmax = INFINITY;
    
    // Pour chaque axe (x, y, z)
    for (int i = 0; i < 3; i++) {
        float invD;
        switch(i) {
            case 0: invD = 1.0f / ray.direction.getX(); break;
            case 1: invD = 1.0f / ray.direction.getY(); break;
            case 2: invD = 1.0f / ray.direction.getZ(); break;
        }
        
        float t0, t1;
        float origin, min_i, max_i;
        
        // Récupérer les composantes correspondantes
        switch(i) {
            case 0:
                origin = ray.origin.getX();
                min_i = box.min.getX();
                max_i = box.max.getX();
                break;
            case 1:
                origin = ray.origin.getY();
                min_i = box.min.getY();
                max_i = box.max.getY();
                break;
            case 2:
                origin = ray.origin.getZ();
                min_i = box.min.getZ();
                max_i = box.max.getZ();
                break;
        }
        
        t0 = (min_i - origin) * invD;
        t1 = (max_i - origin) * invD;
        
        
        if (invD < 0.0f) {
            std::swap(t0, t1);
        }
        
        tmin = std::max(tmin, t0);
        tmax = std::min(tmax, t1);
        

        if (tmax <= tmin) {
            return result;  // Pas d'intersection
        }
    }
    
    // Si on arrive ici, il y a intersection
    result.hit = true;
    result.distance = tmin > 0 ? tmin : tmax;
    


    
    result.point = ray.pointAtDistance(result.distance);
    
    // Calcul de la normale
    vec3 center = (box.max + box.min) * 0.5f;
    vec3 p = result.point - center;
    vec3 d = (box.max - box.min) * 0.5f;
    
    float bias = 1.000001f;
    
    if (std::abs(p.getX() / d.getX()) > bias) result.normal = vec3(p.getX() > 0 ? 1 : -1, 0, 0);
    else if (std::abs(p.getY() / d.getY()) > bias) result.normal = vec3(0, p.getY() > 0 ? 1 : -1, 0);
    else result.normal = vec3(0, 0, p.getZ() > 0 ? 1 : -1);
    
    return result;
}

HitInfo Scene::intersect(const Ray& ray) const {
    HitInfo result = {false, INFINITY, vec3(), vec3()};
    

    // Test des intersections avec les sphères
    for (const auto& sphere : spheres) {
        vec3 oc = ray.origin - sphere.center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - sphere.radius * sphere.radius;
        float discriminant = b*b - 4*a*c;
        
        if (discriminant > 0) {
            float t = (-b - sqrt(discriminant)) / (2.0f*a);
            if (t > 0.001f && t < result.distance) {
                result.hit = true;
                result.distance = t;
                result.point = ray.pointAtDistance(t);
                result.normal = (result.point - sphere.center).normalize();
            }
        }
    }

    // Test des intersections avec les boîtes
    for (const auto& box : boxes) {
        HitInfo boxHit = intersectBox(ray, box);
        if (boxHit.hit) {
            if (boxHit.distance < result.distance) {
                result = boxHit;
            }
        }
    }
    
    return result;
} 