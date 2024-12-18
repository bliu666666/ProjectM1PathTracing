#include "camera.h"

// Constructeurs
Camera::Camera()
    : Entity(), fov(90), imageWidth(800), imageHeight(600), direction(0.0, 0.0, 1.0) {}

Camera::Camera(const Position& position, const Position& direction, double fov, unsigned width, unsigned height)
    : Entity(position),
      fov(fov),
      imageWidth(width),
      imageHeight(height),
      direction(direction) {}

// Getters
double Camera::getFov() const { return fov; }
unsigned Camera::getImageWidth() const { return imageWidth; }
unsigned Camera::getImageHeight() const { return imageHeight; }
Position Camera::getDirection() const { return direction; }

// Setters
void Camera::setFov(double fov) { this->fov = fov; }
void Camera::setImageDimensions(unsigned width, unsigned height) {
    imageWidth = width;
    imageHeight = height;
}
void Camera::setDirection(const Position& direction) { this->direction = direction; }
