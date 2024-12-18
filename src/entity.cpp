#include "entity.h"

// Constructors
Entity::Entity() : position(0, 0, 0) {}

Entity::Entity(double x, double y, double z) : position(x, y, z) {}

Entity::Entity(Position p){this->position = p;}

// Getters
Position Entity::getPosition() const {
    return position;
}

// Setters
void Entity::setPosition(const Position& newPosition) {
    position = newPosition;
}

void Entity::setPosition(double x, double y, double z) {
    position.setX(x);
    position.setY(y);
    position.setZ(z);
}
