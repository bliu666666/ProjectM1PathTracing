#ifndef AABB_H
#define AABB_H

#include "object.h"
#include "../src/ray.h"
#include "../src/position.h"
#include <vector>
#include <memory>



class AABB : public Object {
private:
    Position minCorner;
    Position maxCorner;

public:
    AABB(Position minCorner, Position maxCorner, std::shared_ptr<Material> m);
    AABB(Position center, double width, double height, double depth, std::shared_ptr<Material> m);

    bool intersect(const Ray& ray, HitRecord& rec) const override;
};

#endif
