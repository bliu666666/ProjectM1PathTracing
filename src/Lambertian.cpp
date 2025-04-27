#include "Lambertian.h"

// Constructeur
Lambertian::Lambertian(const std::vector<double>& a) : albedo(a) {}

// Méthode emitted (temporaire, pour rendu simple)
std::vector<double> Lambertian::emitted(double u, double v, const Position& p) const {
    return {0.0, 0.0, 0.0}; // Un matériau lambertien n'émet pas de lumière
}

// Implémentation de la diffusion lambertienne
bool Lambertian::scatter(
    const Ray& ray_in,
    const HitRecord& rec,
    std::vector<double>& attenuation,
    Ray& scattered
) const {
    // Direction de diffusion = normale + vecteur aléatoire unitaire
    Position scatter_direction = rec.normal + random_unit_vector();

    // Éviter les directions de diffusion nulles
    if (std::abs(scatter_direction.dot(scatter_direction)) < 1e-8)
        scatter_direction = rec.normal;

    // Créer le rayon diffusé
    scattered = Ray(rec.point, scatter_direction.normalize());
    
    // L'atténuation est la couleur du matériau
    attenuation = albedo;
    
    return true;
} 