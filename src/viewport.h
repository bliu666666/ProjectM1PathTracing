#include "vec3.h"


class viewport{
public:
  // image dimensions
  double height;
  double width;
  // viewport settings
  double focal;
  vec3 origin_cam;
  vec3 horizontal;
  vec3 vertical;
  
  viewport(double h,double w,const vec3& origin) : height(h), width(w), origin_cam(origin),
  horizontal(vec3(w,0,0)),vertical(vec3(0,-h,0)) {}

  vec3 viewport_u(){
    auto viewport_width = horizontal * (width/ height);
    return viewport_width;
  }
  // const
  vec3 viewport_v(){
    return vertical;
  }

  vec3 get_origin() const { return origin_cam; }
  vec3 get_horizontal() const { return horizontal; }
  vec3 get_vertical() const { return vertical; }




}; 
