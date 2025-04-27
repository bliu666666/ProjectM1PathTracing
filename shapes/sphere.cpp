#include "sphere.h"
#include <cmath>
#include <utility>

Sphere::Sphere() : Object(), radius(1) {}

Sphere::Sphere(Position p, double radius, std::shared_ptr<Material> m)
    : Object(p, std::move(m)), radius(radius) {}

double Sphere::getRadius() const { return radius; }

void Sphere::setRadius(double radius) { this->radius = radius; }

bool Sphere::intersect(const Ray& ray, HitRecord& rec) const {
    // Vector entre l'origine du rayon et le centre de la sphère
    Position oc = ray.getOrigin() - position;

    double a = ray.getDirection().dot(ray.getDirection());
    double b = 2.0 * oc.dot(ray.getDirection());
    double c = oc.dot(oc) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; 
    }

    // Calcul des deux racines t0 et t1
    double sqrtd = std::sqrt(discriminant);
    double root = (-b - sqrtd) / (2.0 * a);

    // Vérifier si la première racine est valide
    if (root < 1e-4) {
        root = (-b + sqrtd) / (2.0 * a);
        if (root < 1e-4)
            return false;
    }

    // Remplir le HitRecord
    rec.t = root;
    rec.point = ray.at(rec.t);
    Position outward_normal = (rec.point - position) / radius; 
    rec.setFaceNormal(ray, outward_normal);
    rec.material = material;

    return true;
}

