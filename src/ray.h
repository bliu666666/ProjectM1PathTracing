#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "color.h"


class ray{
  public:
    vec3 origin;
    vec3 direction;

    ray(): origin(vec3(0,0,0)), direction(vec3(0,0,0)){}

    ray(const vec3& origin, const vec3& direction) : origin(origin), direction(direction){}
    
    vec3 get_origin() const {return origin;}
    vec3 get_direction() const {return direction;}
    
    vec3 at(double t) const{
      return origin + t * direction;
    }


};
#endif // !RAY_H
