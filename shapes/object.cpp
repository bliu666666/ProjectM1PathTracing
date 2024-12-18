#include "object.h"

// Constructeurs
Object::Object() : Entity(), color({1.0, 1.0, 1.0}) {} // Couleur par défaut : blanc

Object::Object(Position p, std::vector<double> color)
    : Entity(p) {this->color = color;}

// Définit la couleur de l'objet
void Object::setColor(const std::vector<double>& color) { 
}

// Renvoie la couleur de l'objet
const std::vector<double>& Object::getColor() const {
    return color;
}
