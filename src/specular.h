#pragma once

#include "obj.h"
#include <cmath>

class Specular:public Material
{
    private:
        Vec3 reflect(const Vec3& v,const Vec3& n)const;
    public:
        Vec3 albedo; // reflection Color
        Specular(const Vec3& a):albedo(a){}
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const override;
};