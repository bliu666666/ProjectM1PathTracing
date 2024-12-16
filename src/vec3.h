#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class vec3 {
private:
    float x, y, z;

public:

    vec3();
    vec3(float x, float y, float z);


    float getX() const;
    float getY() const;
    float getZ() const;


    void setX(float x);
    void setY(float y);
    void setZ(float z);


    vec3 operator+(const vec3& other) const;
    vec3 operator-(const vec3& other) const;
    vec3 operator+(float scalar) const;
    vec3 operator-(float scalar) const;
    vec3 operator+(double scalar) const;
    vec3 operator-(double scalar) const;
    vec3 operator+(int scalar) const;
    vec3 operator-(int scalar) const;
    vec3 operator*(double scalar) const;
    vec3 operator*(int scalar) const;
    vec3 operator*(float scalar) const; 
    float dot(const vec3& other) const;
    vec3 cross(const vec3& other) const;
    float magnitude() const; 
    vec3 normalize() const;  


    void print() const;
};

#endif
