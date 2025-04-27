#include "camera.h"
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Camera::Camera(Position lookfrom, Position lookat, Position vup, double vfov, double aspect_ratio)
    : Entity(lookfrom),
      lookfrom(lookfrom),
      lookat(lookat),
      vup(vup),
      vfov(vfov),
      aspect_ratio(aspect_ratio)
{
    double theta = vfov * M_PI / 180.0;
    double h = std::tan(theta / 2.0);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    Position w = (lookfrom - lookat).normalize();
    Position u = vup.cross(w).normalize();
    Position v = w.cross(u);

    origin = lookfrom;
    horizontal = u * viewport_width;
    vertical = v * viewport_height;
    lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - w;
}


Ray Camera::get_ray(double s, double t) const {
    Position direction = lower_left_corner + horizontal * s + vertical * t - origin;
    return Ray(origin, direction.normalize());
}
