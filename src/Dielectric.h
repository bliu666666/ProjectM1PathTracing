#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"
#include "utils.h"

class Dielectric : public Material {
public:
    double ir;

    Dielectric(double index_of_refraction);

    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        std::vector<double>& attenuation,
        Ray& scattered
    ) const override;

private:
    static double reflectance(double cosine, double ref_idx_ratio);
};

#endif // DIELECTRIC_H 