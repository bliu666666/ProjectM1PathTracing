#pragma once

#include "obj.h"
#include <cmath>
#include "lambertian.h"

// Dielectric material class:simulate transparent materials such as glass,water,etc
class Dielectric:public Material
{
    private:
        Vec3 reflect(const Vec3& v,const Vec3& n)const;
        Vec3 refract(Vec3& uv,const Vec3& n,double etai_over_etat)const; // refraction vector(take into account refractive index)
        double schlick(double cosine,double ref_idx)const; // Fresnel reflectivity estimation(Schlick approximation)

    public:
        double ref_idx;  // refractive index
        Dielectric(double ri):ref_idx(ri){}
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const;
};