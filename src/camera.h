#pragma once

#include "ray.h"
#include "cuda_compat.h"

class Camera
{
    public:
        Vec3 origin;
        Vec3 lower_left_corner;//bottom left corner of the viewport
        Vec3 horizontal;//width of the viewport
        Vec3 vertical;//height of the viewport

        CUDA_HOST_DEVICE Camera(const Vec3& origin,const Vec3& lookat, const Vec3& v_up, const double v_fov, const double aspect);
        CUDA_HOST_DEVICE Ray getRay(const double s,const double t)const;//generate a ray into the scene for a given pixel coordinate
};