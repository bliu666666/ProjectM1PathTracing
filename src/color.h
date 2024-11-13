#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;

void write_color(std::ostream& out,const color& pixel_color){
  int rbyte = static_cast<int>(255.999 * pixel_color.x);
  int gbyte = static_cast<int>(255.999 * pixel_color.y);
  int bbyte = static_cast<int>(255.999 * pixel_color.z);
  
  out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;

}

#endif // !COLOR_H
