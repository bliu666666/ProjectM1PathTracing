#ifndef HITRECORD_H
#define HITRECORD_H

#include "position.h"
#include "ray.h"
#include <memory>

class Material; // Forward declaration

struct HitRecord {
    Position point;     // Point d'intersection
    Position normal;    // Normale à la surface au point d'intersection
    double t;          // Distance le long du rayon
    bool frontFace;    // Le rayon frappe-t-il la face avant ou arrière ?
    std::shared_ptr<Material> material;  // Matériau de l'objet touché

    // Définit la normale en fonction de la direction du rayon
    inline void setFaceNormal(const Ray& ray, const Position& outwardNormal);
};

inline void HitRecord::setFaceNormal(const Ray& ray, const Position& outwardNormal) {
    frontFace = ray.getDirection().dot(outwardNormal) < 0;
    normal = frontFace ? outwardNormal : outwardNormal * -1.0;
}

#endif // HITRECORD_H 