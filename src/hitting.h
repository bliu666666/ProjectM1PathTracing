#ifndef HITTING_H
#define HITTING_H

#include "ray.h"
#include "sphere.h"


class Hitting {
public:
    static vec3 trace(const ray& ray, const std::vector<Sphere>& spheres) {
        float closest_t = INFINITY;
        const Sphere* closest_sphere = nullptr;

        for (const auto& sphere : spheres) {
            float t;
            if (sphere.hit(ray, t) && t < closest_t) {
                closest_t = t;
                closest_sphere = &sphere;
            }
        }

        if (closest_sphere) {
            vec3 hit_post = ray.get_origin() + closest_t * ray.get_direction();
            vec3 N = closest_sphere->normal(hit_post);
            return 0.5 * vec3(N.x + 1,N.y +1,N.z +1);
        }
        return sky(ray); // Background color
    }
private:

    static vec3 sky(const ray& ray) {
        vec3 unit_direction = ray.get_direction().normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);
       
        vec3 color_top(0.5f, 0.7f, 1.0f); 
        vec3 color_bottom(1.0f, 1.0f, 1.0f); 

        return (1.0f - t) * color_bottom + t * color_top;}
};




#endif // !HITTING_H
