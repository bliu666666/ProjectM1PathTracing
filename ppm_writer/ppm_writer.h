#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char *img2rgb(unsigned w, unsigned h, double *img);
void writePPM_normal(char *path, unsigned w, unsigned h, unsigned char *img);
void writePPM_MLT(const char* path, unsigned w, unsigned h, const double* img);
