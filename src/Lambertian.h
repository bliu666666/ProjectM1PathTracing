#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "utils.h"
#include <vector>


class Lambertian : public Material {
public:
    std::vector<double> albedo; // Couleur de base du matériau

    
    Lambertian(const std::vector<double>& a);

    
    virtual std::vector<double> emitted(double u, double v, const Position& p) const override;

   
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        std::vector<double>& attenuation,
        Ray& scattered
    ) const override;
};

#endif // LAMBERTIAN_H 