#include "viewport.h"
#include <cmath>


vec3 viewport::pixelPosition(int i, int j, vec3 qx, vec3 qy) const {
    vec3 initialPosition=C-cam.b*(hx/2)-cam.v*(hy/2);
    return initialPosition+qx.operator*(i-1)+qy.operator*(j-1);
}

vec3 viewport::computeRayVector(int i, int j) const {
    float u = (float)i / (k - 1);
    float v = (float)j / (m - 1);
    
    // Convertir les coordonnées du pixel en coordonnées dans l'espace de la caméra
    float x = (2.0f * u - 1.0f) * tan(theta / 2.0f);
    float y = (2.0f * v - 1.0f) * tan(theta / 2.0f) * ((float)m / k);
    
    // Direction du rayon dans l'espace de la caméra
    vec3 direction = vec3(x, y, -1.0f).normalize();
    
    
    return direction;
}