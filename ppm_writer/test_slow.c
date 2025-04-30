#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ppm_writer.h"

// Fonction pour ralentir l'exécution
void slow_down() {
    int i, j;
    double sum = 0.0;
    for (i = 0; i < 10000000; i++) {
        for (j = 0; j < 10; j++) {
            sum += i * j;
        }
    }
}

int main() {
    int width = 800;
    int height = 600;
    
    // Allouer de la mémoire pour l'image
    unsigned char* image = (unsigned char*)malloc(width * height * 3);
    
    // Générer une image simple
    int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            image[index] = x % 256;        // Rouge
            image[index + 1] = y % 256;    // Vert
            image[index + 2] = (x+y) % 256; // Bleu
        }
    }
    
    // Ralentir l'exécution pour le profiling
    slow_down();
    
    // Écrire l'image dans un fichier PPM
    writePPM("test.ppm", width, height, image);
    
    // Libérer la mémoire
    free(image);
    
    return 0;
}
