#include "sphere.h"
#include <cmath>

// Constructeurs
Sphere::Sphere() : Object(), radius(1) {}

Sphere::Sphere(Position p, double radius, std::vector<double> color)
    : Object(p,color), radius(radius) {}

// Getters
double Sphere::getRadius() const { return radius; }
const std::vector<double>& Sphere::getColor() const { return color; }

// Setters
void Sphere::setRadius(double radius) { this->radius = radius; }
void Sphere::setColor(const std::vector<double>& color) {
    if (color.size() == 3) {
        this->color = color;
    }
}

bool Sphere::intersect(const Ray& ray, double& t) const {
    // Vector entre l'origine du rayon et le centre de la sphère
    Position oc = ray.getOrigin() - position;

    double a = ray.getDirection().dot(ray.getDirection());
    double b = 2.0 * oc.dot(ray.getDirection());
    double c = oc.dot(oc) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // Pas d'intersection
    }

    // Calcul des deux racines t0 et t1
    double t0 = (-b - std::sqrt(discriminant)) / (2.0 * a);
    double t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);

    // Choisir la racine valide la plus proche
    if (t0 > 1e-4) {
        t = t0;
    } else if (t1 > 1e-4) {
        t = t1;
    } else {
        return false; // Les deux intersections sont derrière le rayon
    }

    return true;
}

