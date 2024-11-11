#pragma once

#include "ray.h"

class Object
{
    public:
        virtual ~Object()=default;
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t)const=0;
        virtual Vec3 getCenter()const=0;//get the position of the center of the object
};