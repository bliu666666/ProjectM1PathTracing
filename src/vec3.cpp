#include "vec3.h"


vec3::vec3() : x(0), y(0), z(0) {}
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}


float vec3::getX() const { return x; }
float vec3::getY() const { return y; }
float vec3::getZ() const { return z; }


void vec3::setX(float x) { this->x = x; }
void vec3::setY(float y) { this->y = y; }
void vec3::setZ(float z) { this->z = z; }


vec3 vec3::operator+(const vec3& other) const {
    return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3& other) const {
    return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator+(float scalar) const {
    return vec3(x + scalar, y + scalar, z + scalar);
}

vec3 vec3::operator-(float scalar) const {
    return vec3(x - scalar, y - scalar, z - scalar);
}

vec3 vec3::operator+(double scalar) const {
    return vec3(x + scalar, y + scalar, z + scalar);
}

vec3 vec3::operator-(double scalar) const {
    return vec3(x - scalar, y - scalar, z - scalar);
}

vec3 vec3::operator+(int scalar) const {
    return vec3(x + scalar, y + scalar, z + scalar);
}

vec3 vec3::operator-(int scalar) const {
    return vec3(x - scalar, y - scalar, z - scalar);
}


vec3 vec3::operator*(double scalar) const {
    return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::operator*(int scalar) const {
    return vec3(x * scalar, y * scalar, z * scalar);
}


vec3 vec3::operator*(float scalar) const {
    return vec3(x * scalar, y * scalar, z * scalar);
}


float vec3::dot(const vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

vec3 vec3::cross(const vec3& other) const {
    return vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float vec3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize() const {
    float mag = magnitude();
    if (mag == 0) return vec3(0, 0, 0);
    return vec3(x / mag, y / mag, z / mag);
}


void vec3::print() const {
    std::cout << "vec3(" << x << ", " << y << ", " << z << ")" << std::endl;
}
