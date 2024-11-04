#pragma once

#include <vector>
#include <limits>
#include <cfloat>
#include "obj.h"
#include "sphere.h"
#include "AABB.h"

struct HitInfo
{
    Object* hitObject;//object that collided
    Vec3 intersection;
    double distance;

    HitInfo():hitObject(nullptr),distance(DBL_MAX){}
};

HitInfo findFirstCollision(const Ray& ray,const std::vector<Object*>& scene)
{
    HitInfo closestHit;
    double tmin=DBL_MAX;//the initial minimum distance is set to infinity
    for (auto* obj:scene)
    {
        Vec3 intersection;
        double t;
        if (obj->intersect(ray,0.001f,tmin,intersection,t))
        {
            closestHit.hitObject=obj;
            closestHit.intersection=intersection;
            closestHit.distance=t;
        }
    }
    return closestHit;
}