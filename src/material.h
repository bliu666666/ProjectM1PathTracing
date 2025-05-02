#pragma once

#include "hitInfo.h"
#include "cuda_compat.h"

class Material
{
    public:
        // Détermine comment la lumière est diffusée par le matériau
        CUDA_HOST virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const=0;
        CUDA_HOST virtual ~Material()=default;
};