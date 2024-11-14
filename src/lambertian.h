#pragma once

#include "obj.h"
#include <random>

//generate a random floating point number to generate a slightly different direction for each ray
inline double randomFloat()
{
    static std::uniform_real_distribution<float> dist(0.0,1.0);
    static std::default_random_engine generator;
    return dist(generator);
}

class Lambertian:public Material
{
    public:
        Vec3 albedo;//diffuse reflectance
        Lambertian(const Vec3& a):albedo(a){}
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const;
    private:
        //generate a random unit vector to simulate the random scattering effect of Lambertian reflection
        Vec3 randomUnitVector()const;
};