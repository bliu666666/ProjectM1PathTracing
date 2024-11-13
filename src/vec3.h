#ifndef VEC3_H
#define VEC3_H


#include <math.h>
#include <iostream>

class vec3{
  public:
    double x,y,z;
    vec3(double x, double y , double z) : x(x),y(y),z(z){}
    // create operator for 3d vector
    vec3 operator+(const vec3& v) const{
      return vec3(x + v.x, y + v.y, z + v.z);
    } 
    vec3 operator-(const vec3& v) const {
      return vec3(x - v.x, y - v.y, z - v.z);
    }
    vec3 operator*(double t) const {
      return vec3(x * t, y * t, z * t);
    }
    vec3 operator/(double t) const {
      return vec3(x*1/t,y*1/t,z*1/t);
    };
    double length() const {
      return sqrt(x*x+y*y+z*z);
    }
    double length_squared() const {
      return sqrt(length_squared());
    }
    vec3 normalize() const {
      double len = length();
      return vec3(x / len , y / len, z / len);
    }
    double dot(const vec3& v) const{
      return x* v.x + y * v.y + z * v.z ;
    }
    vec3 cross_product(const vec3& v) const {
      return vec3(
          y * v.z - z * v.y,
          z * v.x - x * v.z,
          x * v.y - y * v.x
          );
    }
};

inline vec3 operator*(double t,const vec3& v){
  return vec3(t*v.x, t*v.y, t*v.z);
}
#endif // !VEC3_H

