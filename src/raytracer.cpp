#include "raytracer.h"
#include "sphere.h"
#include "material.h"
#include "lambertian.h"
#include <random>
#include <fstream>
#include <algorithm>
#include <limits>

RayTracer::RayTracer(int width, int height) : image_width(width), image_height(height) {}

double RayTracer::random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

void RayTracer::render(const Viewport& vp, float aspect_ratio, const std::string& filename, bool anti_aliasing) {
    std::ofstream file(filename);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";
    
    // Create a larger sphere closer to the camera
    Material* material = new Lambertian(Vec3(0.7, 0.3, 0.3));
    Sphere sphere(material, 0.8, Vec3(0, 0, -1));
    
    for (int j = image_height-1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Vec3 pixel_color(0, 0, 0);
            double t;
            
            if (anti_aliasing) {
                // With anti-aliasing: take many more samples per pixel
                const int samples_per_pixel = 500; 
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    Ray r = vp.get_ray(u, v);
                    
                    Vec3 intersection;
                    Vec3 normal;
                    if (sphere.intersect(r, 0.001, std::numeric_limits<double>::infinity(), intersection, t, normal)) {
                        Vec3 hit_point = r.at(t);
                        pixel_color = pixel_color + 0.5 * (normal + Vec3(1,1,1));
                    } else {
                        Vec3 unit_direction = r.direction.normalize();
                        auto a = 0.5*(unit_direction.y + 1.0);
                        pixel_color = pixel_color + (1.0-a)*Vec3(1.0, 1.0, 1.0) + a*Vec3(0.5, 0.7, 1.0);
                    }
                }
                pixel_color = pixel_color / samples_per_pixel;
            } else {
                // Without anti-aliasing: one sample per pixel
                auto u = double(i) / (image_width-1);
                auto v = double(j) / (image_height-1);
                Ray r = vp.get_ray(u, v);
                
                Vec3 intersection;
                Vec3 normal;
                if (sphere.intersect(r, 0.001, std::numeric_limits<double>::infinity(), intersection, t, normal)) {
                    Vec3 hit_point = r.at(t);
                    pixel_color = 0.8 * (normal + Vec3(1,1,1));
                } else {
                    Vec3 unit_direction = r.direction.normalize();
                    auto a = 0.5*(unit_direction.y + 1.0);
                    pixel_color = (1.0-a)*Vec3(1.0, 1.0, 1.0) + a*Vec3(0.5, 0.7, 1.0);
                }
            }
            
            int ir = static_cast<int>(255.999 * std::clamp(pixel_color.x, 0.0, 0.999));
            int ig = static_cast<int>(255.999 * std::clamp(pixel_color.y, 0.0, 0.999));
            int ib = static_cast<int>(255.999 * std::clamp(pixel_color.z, 0.0, 0.999));
            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    
    file.close();
}