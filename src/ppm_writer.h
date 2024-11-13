#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "color.h"
#include "viewport.h"

// Convert image to RGB format for PPM
unsigned char *img2rgb(unsigned w, unsigned h, double *img) {
  // Allocate memory for the resulting RGB image
  unsigned char *rgbImg = (unsigned char*) malloc(w * h * 3 * sizeof(unsigned char));

  for (unsigned i = 0; i < h; i++) {
    for (unsigned j = 0; j < w; j++) {
      double *pixel = &(img[3 * (i * w + j)]);
      unsigned char *rgbPixel = &(rgbImg[3 * (i * w + j)]);
      rgbPixel[0] = (unsigned char) fmin(255., 255. * pixel[0]);
      rgbPixel[1] = (unsigned char) fmin(255., 255. * pixel[1]);
      rgbPixel[2] = (unsigned char) fmin(255., 255. * pixel[2]);
    }
  }

  return rgbImg;
}

// Write image to PPM file
void writePPM(const char *path, unsigned w, unsigned h, unsigned char *img) {
  // Open file to write the image
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    printf("Error: Could not open the file.\n");
  } else {
    // Write PPM header
    fprintf(file, "P3\n%u %u\n255\n", w, h);

    // Write pixel data
    for (unsigned i = 0; i < h; i++) {
      for (unsigned j = 0; j < w; j++) {
        unsigned char *pixel = img + 3 * (i * w + j);
        fprintf(file, "%d %d %d ", pixel[0], pixel[1], pixel[2]);
      }
      fprintf(file, "\n");
    }

    fclose(file);
  }
}

// Calculate the color of the ray based on its direction
color ray_color(const ray& r) {
  // Simple background color gradient based on ray direction
  vec3 unit_direction = r.get_direction().normalize();
  
  // Interpolate between the sky color and a background color based on the ray direction.
  double t = 0.5 * (unit_direction.y + 1.0);
  
  color c = (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // White to sky blue
  return c;
}

// Render scene to an image and save as a PPM file
void render(viewport& vp, unsigned width, unsigned height, const char* path) {
  unsigned char* img = (unsigned char*) malloc(width * height * 3 * sizeof(unsigned char));

  for (unsigned j = 0; j < height; j++) {
    for (unsigned i = 0; i < width; i++) {
      // Calculate the ray for each pixel
      double u = double(i) / (width - 1);
      double v = double(j) / (height - 1);
      
      ray r(vp.get_origin(), vp.get_horizontal() * u + vp.get_vertical() * v);
      
      // Get the color for this ray
      color pixel_color = ray_color(r);
      
      // Convert the color to RGB and store it in the image array
      img[3 * (j * width + i)] = (unsigned char) (255.0 * pixel_color.x);
      img[3 * (j * width + i) + 1] = (unsigned char) (255.0 * pixel_color.y);
      img[3 * (j * width + i) + 2] = (unsigned char) (255.0 * pixel_color.z);
    }
  }

  // Write the image to a PPM file
  writePPM(path, width, height, img);
  free(img);
}

#endif // RENDER_H

