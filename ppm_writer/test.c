#include <stdio.h>

#include "ppm_writer.h"

int main(int argc, char* argv[]) {

  /* Test PPM writer */
  unsigned char img[6][3] = {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
    {255, 255, 0}, {255, 255, 255}, {0, 0, 0}
  };
  writePPM("test.ppm", 3, 2, &(img[0][0]));
}
