#include "ppm_writer.h"
#include <algorithm>

/*
Here is an example of PPM file:
3
# ^ Magic number to indicate that this is a PPM file with RGB, ASCII colors.
3 2
# ^ Numbers of columns and rows respectively.
255
# ^ Corresponds to the max color value
# From now on, each values triplet represents a pixel:
255 0 0 0 255 0 0 0 255
255 255 0 255 255 255 0 0 0
*/

unsigned char *img2rgb(unsigned w, unsigned h, double *img) {
  // Allocate resulting image
  unsigned char *rgbImg = (unsigned char*) malloc(w*h*3 * sizeof(unsigned char));
  
  for(unsigned i=0; i<h; i++) {
    for(unsigned j=0; j<w; j++) {
      double *pixel = &(img[3*(i*w + j)]);
      unsigned char *rgbPixel = &(rgbImg[3*(i*w + j)]);
      rgbPixel[0] = (unsigned char) fmin(255., 255.*pixel[0]);
      rgbPixel[1] = (unsigned char) fmin(255., 255.*pixel[1]);
      rgbPixel[2] = (unsigned char) fmin(255., 255.*pixel[2]);
    }
  }

  return rgbImg;
}

void writePPM_normal(char *path, unsigned w, unsigned h, unsigned char *img) {
    // img is a w*h*3 array, stored in row-major mode
    // (ie. unsigned char img[w][h][3])
  
    FILE *file;
    file = fopen(path, "w");
    if (file == NULL) {
      printf("Error: Could not open file.\n");
    } else {
  
      // Write file header
      fprintf(file, "P3\n%u %u\n255\n", w, h);
  
      // Write file content
      for(unsigned i=0; i<h; i++) {
        for(unsigned j=0; j<w; j++) {
          unsigned char *pixel = img + 3 * (i*w + j);
          fprintf(file, "%d %d %d ", pixel[0], pixel[1], pixel[2]);
        }
        fprintf(file, "\n");
      }
  
      fclose(file);
    }
}

void writePPM_MLT(const char* path, unsigned w, unsigned h, const double* img)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Error: Could not open file for writing: %s\n", path);
        return;
    }

    fprintf(file, "P3\n%u %u\n255\n", w, h);

    double exposure = 0.4; // Adjustable brightness control factor

    for (unsigned i = 0; i < h; ++i) {
        for (unsigned j = 0; j < w; ++j) {
            int idx = 3 * (i * w + j);
            
            // Exposure compression
            double r = 1.0 - exp(-img[idx]   * exposure);
            double g = 1.0 - exp(-img[idx+1] * exposure);
            double b = 1.0 - exp(-img[idx+2] * exposure);

            // Gamma correction
            r = pow(std::clamp(r,0.0,1.0), 1.0/2.2);
            g = pow(std::clamp(g,0.0,1.0), 1.0/2.2);
            b = pow(std::clamp(b,0.0,1.0), 1.0/2.2);

            fprintf(file, "%d %d %d ", static_cast<int>(r * 255.99),static_cast<int>(g * 255.99),static_cast<int>(b * 255.99));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}