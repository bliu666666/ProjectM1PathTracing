#pragma once

#include "ray.h"

class Object
{
    public:
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t)const=0;
        virtual ~Object()=default;
};