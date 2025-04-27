#ifndef CAMERA_H
#define CAMERA_H

#include "entity.h"
#include "ray.h"

class Camera : public Entity {
private:
    Position lookfrom;
    Position lookat;
    Position vup;
    double vfov;
    double aspect_ratio;

    Position origin;
    Position horizontal;
    Position vertical;
    Position lower_left_corner;

public:
    Camera(Position lookfrom, Position lookat, Position vup, double vfov, double aspect_ratio);

    Ray get_ray(double s, double t) const;

    ~Camera() = default;
};

#endif // CAMERA_H
