#ifndef ENTITY_H
#define ENTITY_H

#include "position.h"

class Entity {
protected:
    Position position;

public:
    Entity();
    Entity(const Position& p);

    Position getPosition() const;
    void setPosition(const Position& p);
    void setPosition(double x, double y, double z);

    virtual ~Entity() = default;
};

#endif // ENTITY_H
