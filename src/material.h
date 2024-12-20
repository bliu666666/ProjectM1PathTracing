#pragma once

#include "hitInfo.h"
class Material
{
    public:
        // Détermine comment la lumière est diffusée par le matériau
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const=0;
        virtual ~Material()=default;
};