#ifndef CUBE_H
#define CUBE_H

#include "object.h"
#include "../src/ray.h"
#include "../src/position.h"
#include <vector>

class Cube : public Object {
private:
    double sideLength;

public:
    Cube(Position position, double sideLength, std::vector<double> color);

    bool intersect(const Ray& ray, double& t) const override;
};

#endif
