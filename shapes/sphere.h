#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include <vector>
#include <memory>


class Sphere : public Object {
private:
    double radius;
    // std::vector<double> color;

public:
    Sphere();
    Sphere(Position p, double radius, std::shared_ptr<Material> m); 

    double getRadius() const;
    // const std::vector<double>& getColor() const; 

    void setRadius(double radius);
    // void setColor(const std::vector<double>& color); 

    bool intersect(const Ray& ray, HitRecord& rec) const override;

    ~Sphere() = default;
};

#endif // SPHERE_H
