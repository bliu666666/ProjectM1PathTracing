#ifndef PPM_WRITER_H
#define PPM_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Déclaration des fonctions
unsigned char *img2rgb(unsigned w, unsigned h, double *img);
void writePPM(char *path, unsigned w, unsigned h, unsigned char *img);

#ifdef __cplusplus
}
#endif

#endif // PPM_WRITER_H

