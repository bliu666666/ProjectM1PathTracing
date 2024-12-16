#include "viewport.h"
#include <cmath>


vec3 viewport::pixelPosition(vec3 initialPosition, int i, int j, vec3 qx, vec3 qy){
    return initialPosition+qx.operator*(i-1)+qy.operator*(j-1);
}