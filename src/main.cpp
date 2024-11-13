#include <iostream> 
#include "vec3.h"
#include "ppm_writer.h"

int main(void){
  auto aspect_ratio = 16.0 / 9.0;
  int image_width = 400;
  int image_height = static_cast<int>(image_width/aspect_ratio);
  vec3 origin(0.0,0.0,0.0);
  // create viewport 
  viewport vp(image_height,image_width, origin);

  // render
  
  render(vp,image_width,image_height,"./image.ppm");

  std::clog << "\rDone.                 \n";

  return 0;
}
