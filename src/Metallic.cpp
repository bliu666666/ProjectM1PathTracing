#include "Metallic.h"

Metallic::Metallic(const std::vector<double>& a, double f) 
    : albedo(a), fuzz(f < 1.0 ? f : 1.0) {}


std::vector<double> Metallic::emitted(double u, double v, const Position& p) const {
    return {0.0, 0.0, 0.0};
}


bool Metallic::scatter(
    const Ray& ray_in,
    const HitRecord& rec,
    std::vector<double>& attenuation,
    Ray& scattered
) const {
    
    Position reflected = reflect(ray_in.getDirection().normalize(), rec.normal);
    
    // Ajout d'un vecteur aléatoire pour le flou si fuzz > 0
    Position scatter_direction = reflected;
    if (fuzz > 0.0) {
        scatter_direction = scatter_direction + random_in_unit_sphere() * fuzz;
    }
    
    scattered = Ray(rec.point, scatter_direction.normalize());
    
    attenuation = albedo;
    
    return (scatter_direction.dot(rec.normal) > 0);
} 