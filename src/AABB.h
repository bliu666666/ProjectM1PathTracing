#pragma once

#include "obj.h"
#include <utility>

class AABB:public Object
{
    public:
        Vec3 min;//minimum coordinate
        Vec3 max;//maximum coordiante

        AABB(const Vec3& min,const Vec3& max):min(min),max(max){}
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t)const;//check if the ray intersects the bounding box
};