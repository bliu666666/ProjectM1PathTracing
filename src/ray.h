#pragma once

#include "vec3.h"
#include "cuda_compat.h"

class Ray
{
    public:
        Vec3 origin;
        Vec3 direction;

        CUDA_HOST_DEVICE Ray(){}
        CUDA_HOST_DEVICE Ray(const Vec3& origin,const Vec3& direction):origin(origin),direction(direction.normalize()){}
        CUDA_HOST_DEVICE Vec3 at(const double t)const;//get the point of the ray at parameter t
};