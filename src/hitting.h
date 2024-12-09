#ifndef HITTING_H
#define HITTING_H

#include "ray.h"
#include "sphere.h"
#include "random.h"    
#include "ray_color.h"

class Hitting {
public:
    static vec3 trace(const ray& r, const std::vector<Sphere>& spheres, int depth) {
        if (depth <= 0) {
            return vec3(0, 0, 0);
        }

        float closest_t = INFINITY;
        const Sphere* closest_sphere = nullptr;

        for (const auto& sphere : spheres) {
            float t;
            if (sphere.hit(r, t) && t < closest_t) {
                closest_t = t;
                closest_sphere = &sphere;
            }
        }

        if (closest_sphere) {
            vec3 hit_point = r.get_origin() + closest_t * r.get_direction();
            vec3 N = closest_sphere->normal(hit_point);
            
            // Lambertian reflection
            vec3 target = hit_point + N + random_unit_vector();
            vec3 scattered_direction = (target - hit_point).normalize();
            
            // Recursive ray trace with attenuation
            ray scattered_ray(hit_point, scattered_direction);
            return 0.5 * closest_sphere->color * trace(scattered_ray, spheres, depth - 1);
        }
        return sky(r); // Background color
    }
private:

    static vec3 sky(const ray& r) {
        vec3 unit_direction = r.get_direction().normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);
       
        vec3 color_top(0.5f, 0.7f, 1.0f); 
        vec3 color_bottom(1.0f, 1.0f, 1.0f); 

        return (1.0f - t) * color_bottom + t * color_top;}
};




#endif // !HITTING_H
