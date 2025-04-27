#include "entity.h"

// Constructors
Entity::Entity() : position(0, 0, 0) {}

Entity::Entity(const Position& p) : position(p) {}

// Getters
Position Entity::getPosition() const {
    return position;
}

// Setters
void Entity::setPosition(const Position& p) {
    position = p;
}

void Entity::setPosition(double x, double y, double z) {
    position.setX(x);
    position.setY(y);
    position.setZ(z);
}
