#include "ray.h"

// Constructors
Ray::Ray() : origin(0, 0, 0), direction(0, 0, 1) {}

Ray::Ray(const Position& origin, const Position& direction)
    : origin(origin), direction(direction) {}

// Getters
Position Ray::getOrigin() const { return origin; }
Position Ray::getDirection() const { return direction; }

// Setters
void Ray::setOrigin(const Position& origin) { this->origin = origin; }
void Ray::setDirection(const Position& direction) { this->direction = direction.normalize(); }
