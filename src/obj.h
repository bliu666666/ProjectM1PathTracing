#pragma once

#include "ray.h"
#include "vec3.h"
#include "material.h"

class Object
{
    public:
        Material* material; //add a material pointer

        Object(Material* mat):material(mat){}
        virtual ~Object()=default;
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const=0;
        virtual Vec3 getCenter()const=0;//get the position of the center of the object
};