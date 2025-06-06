#pragma once

#include <cfloat>

class Object;

struct HitInfo
{
    Object* hitObject;//object that collided
    Vec3 intersection;
    double distance;
    Vec3 normal;//normal vector
    bool front_face;

    HitInfo():hitObject(nullptr),distance(DBL_MAX){}
    // Set the normal direction to make sure it points to the camera side
    void setFaceNormal(const Ray& ray,Vec3& outward_normal) 
    {
        front_face=ray.direction.dot(outward_normal)<0;
        normal=front_face?outward_normal:-outward_normal;
    }
};