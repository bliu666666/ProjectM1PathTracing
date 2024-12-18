#include "position.h"
#include <cmath> 

// Constructeurs
Position::Position() : x(0), y(0), z(0) {}

Position::Position(double x, double y, double z) : x(x), y(y), z(z) {}

// Getters
double Position::getX() const { return x; }
double Position::getY() const { return y; }
double Position::getZ() const { return z; }

// Setters
void Position::setX(double val) { this->x = val; }
void Position::setY(double val) { this->y = val; }
void Position::setZ(double val) { this->z = val; }
void Position::set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

// Opérateurs mathématiques
Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y, z + other.z);
}

Position Position::operator-(const Position& other) const {
    return Position(x - other.x, y - other.y, z - other.z);
}

Position Position::operator*(double scalar) const {
    return Position(x * scalar, y * scalar, z * scalar);
}

Position Position::operator/(double scalar) const {
    return Position(x / scalar, y / scalar, z / scalar);
}

Position& Position::operator+=(const Position& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Position& Position::operator-=(const Position& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Position& Position::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Position& Position::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Calcul de la distance entre deux positions
double Position::distance(const Position& other) const {
    return std::sqrt(std::pow(x - other.x, 2) +
                     std::pow(y - other.y, 2) +
                     std::pow(z - other.z, 2));
}

double Position::dot(const Position& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Position Position::cross(const Position& other) const {
    return Position(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

double Position::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Position Position::normalize() const {
    double magnitude = norm();
    if (magnitude == 0.0) {
        return Position(0.0, 0.0, 0.0);
    }
    return *this / magnitude;
}
