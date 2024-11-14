#pragma once

#include <cfloat>

class Object;

struct HitInfo
{
    Object* hitObject;//object that collided
    Vec3 intersection;
    double distance;
    Vec3 normal;//normal vector

    HitInfo():hitObject(nullptr),distance(DBL_MAX){}
};