#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "material.h"
#include <vector>


class Emissive : public Material {
public:
    std::vector<double> emitColor;


    Emissive(const std::vector<double>& color);

    
    virtual std::vector<double> emitted(double u, double v, const Position& p) const override;

    
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        std::vector<double>& attenuation,
        Ray& scattered
    ) const override;
};

#endif // EMISSIVE_H 