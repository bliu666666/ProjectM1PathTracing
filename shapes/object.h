#ifndef OBJECT_H
#define OBJECT_H

#include "../src/entity.h"
#include "../src/ray.h"
#include "../src/material.h"
#include "../src/hitrecord.h"
#include <vector>
#include <memory>

class Object : public Entity {
public:
    // Constructeurs
    Object();
    Object(Position p, std::shared_ptr<Material> m);

    // Méthode pour obtenir le matériau
    std::shared_ptr<Material> getMaterial() const;

    // Méthode d'intersection mise à jour pour utiliser HitRecord
    virtual bool intersect(const Ray& ray, HitRecord& rec) const = 0;

    virtual ~Object() = default;

protected:
    std::shared_ptr<Material> material;
};

#endif // OBJECT_H
