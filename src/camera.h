#ifndef CAMERA_H
#define CAMERA_H

#include "entity.h"

class Camera : public Entity {
private:
    double fov; 
    unsigned imageWidth; 
    unsigned imageHeight; 
    Position direction; 

public:
    // Constructeurs
    Camera();
    Camera(const Position& position, const Position& direction, double fov, unsigned width, unsigned height);

    // Getters
    double getFov() const;
    unsigned getImageWidth() const;
    unsigned getImageHeight() const;
    Position getDirection() const;

    // Setters
    void setFov(double fov);
    void setImageDimensions(unsigned width, unsigned height);
    void setDirection(const Position& direction);

    ~Camera() = default;
};

#endif // CAMERA_H
