#ifndef OBJECT_H
#define OBJECT_H

#include "../src/entity.h"
#include "../src/ray.h"
#include <vector>

class Object : public Entity {
public:
    // Constructeurs
    Object();
    Object(Position p, std::vector<double> color);

    // Définit la couleur de l'objet
    void setColor(const std::vector<double>& color);

    // Renvoie la couleur de l'objet
    const std::vector<double>& getColor() const;

    virtual bool intersect(const Ray& ray, double& t) const = 0;

    virtual ~Object() = default;

private:
    std::vector<double> color; 
};

#endif // OBJECT_H
