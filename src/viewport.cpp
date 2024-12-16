#include "viewport.h"
#include <cmath>


vec3 viewport::pixelPosition(int i, int j, vec3 qx, vec3 qy){
    vec3 initialPosition=C-cam.b*(hx/2)-cam.v*(hy/2);
    return initialPosition+qx.operator*(i-1)+qy.operator*(j-1);
}