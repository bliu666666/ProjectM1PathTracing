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
void writePPM(char *path, unsigned w, unsigned h, unsigned char *img) {
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
  double t = 0.5 * (unit_direction.y + 1.0); // Interpolation based on Y-axis (ray direction)
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // Mix between white and blue
}

// Rendering function that generates the image
void render(viewport& vp, unsigned image_width, unsigned image_height, const char* output_file) {
  // Create an array to store image data
  double* img = new double[image_width * image_height * 3];

  // Calculate color for each pixel
  for (unsigned j = 0; j < image_height; ++j) {
    for (unsigned i = 0; i < image_width; ++i) {
      // Compute ray for each pixel
      double u = double(i) / (image_width - 1);
      double v = double(j) / (image_height - 1);

      // Generate ray from camera origin based on (u, v)
      ray r(vp.get_origin(), vp.get_horizontal() * u + vp.get_vertical() * v - vp.get_origin());
      
      // Calculate pixel color based on ray intersection
      color pixel_color = ray_color(r);

      // Store the color in the image array
      img[3 * (j * image_width + i)] = pixel_color.x;
      img[3 * (j * image_width + i) + 1] = pixel_color.y;
      img[3 * (j * image_width + i) + 2] = pixel_color.z;
    }
  }

  // Convert the image data to RGB format
  unsigned char* rgb_img = img2rgb(image_width, image_height, img);

  // Write the image to a PPM file
  writePPM(output_file, image_width, image_height, rgb_img);

  // Free memory
  delete[] img;
  delete[] rgb_img;
}

#endif // RENDER_H

