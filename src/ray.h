#ifndef RAY_H
#define RAY_H

#include "position.h"

class Ray {
private:
    Position origin;
    Position direction; // Must be normalized

public:
    // Constructors
    Ray();
    Ray(const Position& origin, const Position& direction);

    // Getters
    Position getOrigin() const;
    Position getDirection() const;

    // Setters
    void setOrigin(const Position& origin);
    void setDirection(const Position& direction);

    ~Ray() = default;
};

#endif // RAY_H
