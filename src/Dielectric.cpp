#include "Dielectric.h"
#include <cmath>

Dielectric::Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

double Dielectric::reflectance(double cosine, double ref_idx_ratio) {
    auto r0 = (1.0 - ref_idx_ratio) / (1.0 + ref_idx_ratio);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
}

bool Dielectric::scatter(
    const Ray& ray_in,
    const HitRecord& rec,
    std::vector<double>& attenuation,
    Ray& scattered
) const {
    attenuation = {1.0, 1.0, 1.0};
    double refraction_ratio = rec.frontFace ? (1.0 / ir) : ir;

    Position unit_direction = ray_in.getDirection().normalize();
    double cos_theta = std::min(1.0, (-unit_direction).dot(rec.normal));
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    // Vérifie la réflexion interne totale (TIR)
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Position direction;

    // Calcule la réflectance de Fresnel ou si TIR
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
        direction = reflect(unit_direction, rec.normal);
    } else {
        direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = Ray(rec.point, direction.normalize());
    return true; 
} 