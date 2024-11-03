#pragma once

#include "vec3.h"

class Ray
{
    public:
        Vec3 origin;
        Vec3 direction;

        Ray(){}
        Ray(const Vec3& origin,const Vec3& direction):origin(origin),direction(direction.normalize()){}
        Vec3 at(const double t)const;//get the point of the ray at parameter t
};