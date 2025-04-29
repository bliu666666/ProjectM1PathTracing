#include "lambertian.h"

bool Lambertian::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    static thread_local unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count() + omp_get_thread_num() * 7919;
    Vec3 scatter_direction=randomInHemisphere(hit.normal,seed);
    if (scatter_direction.nearZero()) {
        scatter_direction=hit.normal;
    }
    scattered=Ray(hit.intersection,scatter_direction);
    attenuation=albedo;
    return true;
}

Vec3 Lambertian::randomUnitVector(unsigned int& seed)const
{
    double a=fastRandomDouble(seed)*2.0*M_PI;
    double z=fastRandomDouble(seed)*2.0-1.0;
    double r=std::sqrt(1.0-z * z);
    return Vec3(r*std::cos(a),r*std::sin(a),z);
}

Vec3 Lambertian::randomInHemisphere(const Vec3& normal,unsigned int& seed)const
{
    Vec3 randomVector=randomUnitVector(seed);
    return randomVector.dot(normal)>0.0?randomVector:-randomVector;
}