#pragma once
#include "obj.h"

// Emissive materials, which represent self-luminous surfaces (like light sources)
class Emissive : public Material 
{
    public:
        Vec3 emission;
        Emissive(const Vec3& emit):emission(emit){}
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const override;
        Vec3 emitted()const;
};
