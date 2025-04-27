#ifndef UTILS_H
#define UTILS_H

#include "position.h"
#include <random>
#include <cmath>

inline double random_double() {
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());
    thread_local std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

inline Position random_in_unit_sphere() {
    while (true) {
        Position p(
            2.0 * random_double() - 1.0,
            2.0 * random_double() - 1.0,
            2.0 * random_double() - 1.0
        );
        if (p.dot(p) < 1.0) return p;
    }
}

inline Position random_unit_vector() {
    return random_in_unit_sphere().normalize();
}

inline Position random_in_hemisphere(const Position& normal) {
    Position in_unit_sphere = random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0)
        return in_unit_sphere;
    else
        return in_unit_sphere * -1.0;
}

inline Position reflect(const Position& v, const Position& n) {
    return v - n * 2.0 * v.dot(n);
}

inline Position refract(const Position& uv, const Position& n, double etai_over_etat) {
    double cos_theta = std::min(1.0, (-uv).dot(n));
    Position r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    Position r_out_parallel = n * (-std::sqrt(std::fabs(1.0 - r_out_perp.dot(r_out_perp))));
    return r_out_perp + r_out_parallel;
}

#endif // UTILS_H 