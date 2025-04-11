#include "emissive.h"

bool Emissive::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    return false; // No more scattering, just terminate the path
}

Vec3 Emissive::emitted()const
{
    return emission;
}