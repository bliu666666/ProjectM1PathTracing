#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include <vector> 

class Sphere : public Object {
private:
    double radius;
    std::vector<double> color; 

public:
    // Constructeurs
    Sphere();
    Sphere(Position p, double radius, std::vector<double> color);

    // Getters
    double getRadius() const;
    const std::vector<double>& getColor() const; 

    // Setters
    void setRadius(double radius);
    void setColor(const std::vector<double>& color); 


    bool intersect(const Ray& ray, double& t) const override;

    ~Sphere() = default;
};

#endif // SPHERE_H
