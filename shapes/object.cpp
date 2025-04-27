#include "object.h"
#include <utility> // Pour std::move

// Constructeurs
Object::Object() : Entity(), material(nullptr) {} // Matériau par défaut : nul

Object::Object(Position p, std::shared_ptr<Material> m)
    : Entity(p), material(std::move(m)) {}

// Renvoie le matériau de l'objet
std::shared_ptr<Material> Object::getMaterial() const {
    return material;
}

// Les méthodes setColor et getColor sont supprimées car remplacées par le matériau
