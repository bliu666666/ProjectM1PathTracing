#ifndef METALLIC_H
#define METALLIC_H

#include "material.h"
#include "utils.h"
#include <vector>

// Matériau Métallique (réflexion spéculaire)
class Metallic : public Material {
public:
    std::vector<double> albedo; // Couleur de base du matériau
    double fuzz; // Facteur de flou pour la réflexion (0 = miroir parfait, 1 = très flou)

    // Constructeur
    Metallic(const std::vector<double>& a, double f = 0.0);

    // Un matériau métallique n'émet pas de lumière
    virtual std::vector<double> emitted(double u, double v, const Position& p) const override;

    // Implémentation de la réflexion métallique
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        std::vector<double>& attenuation,
        Ray& scattered
    ) const override;
};

#endif // METALLIC_H 