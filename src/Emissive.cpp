#include "Emissive.h"


Emissive::Emissive(const std::vector<double>& color) : emitColor(color) {}


std::vector<double> Emissive::emitted(double u, double v, const Position& p) const {
    return emitColor;
}


bool Emissive::scatter(
    const Ray& ray_in,
    const HitRecord& rec,
    std::vector<double>& attenuation,
    Ray& scattered
) const {
    return false; 
} 