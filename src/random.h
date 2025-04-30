#ifndef RANDOM_H
#define RANDOM_H

#include "vec3.h"
#include <random>

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline vec3 random_unit_vector() {
    double a = random_double() * 2 * M_PI;
    double z = random_double() * 2 - 1;
    double r = sqrt(1 - z * z);
    return vec3(r * cos(a), r * sin(a), z);
}

#endif
