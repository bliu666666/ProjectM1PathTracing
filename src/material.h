#pragma once

#include "hitInfo.h"
class Material
{
    public:
        //determine how to scatter incoming light
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const=0;
        virtual ~Material()=default;
};