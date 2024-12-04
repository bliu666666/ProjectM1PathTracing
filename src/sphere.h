#pragma once

#include "obj.h"

class Sphere:public Object
{
    public:
        double r;
        Vec3 center;

        Sphere(Material* mat,const double r,const Vec3& center):Object(mat),r(r),center(center){}
        virtual Vec3 getCenter()const;
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const;//check if the ray intersects the sphere
};