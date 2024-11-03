#pragma once

#include "ray.h"

class Camara
{
    public:
        Vec3 origin;
        Vec3 lower_left_corner;//bottom left corner of the viewport
        Vec3 horizontal;//width of the viewport
        Vec3 vertical;//height of the viewport

        Camara(const Vec3& origin,const Vec3& lookat, const Vec3& v_up, const double v_fov, const double aspect);
        Ray getRay(const double s,const double t)const;//generate a ray into the scene for a given pixel coordinate
};