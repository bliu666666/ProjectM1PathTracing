#pragma once

#include <vector>
#include <limits>
#include "sphere.h"
#include "AABB.h"
#include "hitInfo.h"

HitInfo findFirstCollision(const Ray& ray,const std::vector<Object*>& scene)
{
    HitInfo closestHit;
    double tmin=DBL_MAX;//the initial minimum distance is set to infinity
    for (auto* obj:scene)
    {
        Vec3 intersection;
        double t;
        Vec3 normal;
        if (obj->intersect(ray,0.001f,tmin,intersection,t,normal))
        {
            closestHit.hitObject=obj;
            closestHit.intersection=intersection;
            closestHit.distance=t;
            closestHit.normal=normal;
        }
    }
    return closestHit;
}