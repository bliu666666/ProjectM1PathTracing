#include "lambertian.h"

bool Lambertian::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    Vec3 scatter_direction=hit.normal+randomUnitVector();
    if (scatter_direction.nearZero()) {
        scatter_direction=hit.normal;
    }
    scattered=Ray(hit.intersection,scatter_direction);
    attenuation=albedo;
    return true;
}

Vec3 Lambertian::randomUnitVector()const
{
    double a=randomFloat()*2.0*M_PI;
    double z=randomFloat()*2.0-1.0;
    double r=std::sqrt(1.0-z * z);
    return Vec3(r*std::cos(a),r*std::sin(a),z);
}