#pragma once

#include "obj.h"
#include <utility>

class AABB:public Object
{
    public:
        Vec3 min;//minimum coordinate
        Vec3 max;//maximum coordiante

        AABB(Material* mat,const Vec3& min,const Vec3& max):Object(mat),min(min),max(max){}
        virtual Vec3 getCenter()const;
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const;//check if the ray intersects the bounding box
};