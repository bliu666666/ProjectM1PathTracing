#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char *img2rgb(unsigned w, unsigned h, double *img);
void writePPM(char *path, unsigned w, unsigned h, unsigned char *img);
