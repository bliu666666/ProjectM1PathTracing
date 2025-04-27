#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "position.h"
#include "hitrecord.h"
#include <vector>
#include <memory>

// Forward declaration pour éviter dépendance cyclique si Material a besoin de HitRecord plus tard
// struct HitRecord;

class Material {
public:
    // Renvoie la couleur émise par le matériau.
    // Par défaut, les matériaux n'émettent pas de lumière.
    // Les coordonnées u, v et le point p pourraient être utilisés pour des textures plus tard.
    virtual std::vector<double> emitted(double u, double v, const Position& p) const {
        return {0.0, 0.0, 0.0}; // Noir
    }

    // Calcule la diffusion du rayon
    // ray_in : rayon incident
    // rec : informations sur l'intersection
    // attenuation : atténuation de la couleur
    // scattered : rayon diffusé
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        std::vector<double>& attenuation,
        Ray& scattered
    ) const = 0;

    virtual ~Material() = default;
};

#endif // MATERIAL_H 