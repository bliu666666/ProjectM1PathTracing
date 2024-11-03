#include "camara.h"

Camara::Camara(const Vec3& origin,const Vec3& lookat, const Vec3& v_up, const double v_fov, const double aspect)
{
    /*
        lookat: The target point that the camera is looking at.
        v_up: The vertical upward direction.
        v_fov: The vertical field of view (FOV) of the camera, in degrees.
        aspect: Aspect ratio (width/height)
    */

    this->origin = origin;
    float theta = v_fov * M_PI / 180.0f; //convert viewing angle to radians
    float halfHeight = tan(theta / 2);
    float halfWidth = aspect * halfHeight;

    Vec3 w = (origin-lookat).normalize();//calculate the direction vector w from the camera position origin to the target point lookat
    Vec3 u = v_up.cross(w).normalize();//calculate the "right" direction vector u of the camera
    Vec3 v = w.cross(u);//calculate the camera's "up" direction vector v

    lower_left_corner = origin - u * halfWidth - v * halfHeight - w;
    horizontal = u * 2.0f * halfWidth;
    vertical = v * 2.0f * halfHeight;
}

Ray Camara::getRay(const double s,const double t)const
{
    return Ray(origin, lower_left_corner + horizontal * s + vertical * t - origin);
}