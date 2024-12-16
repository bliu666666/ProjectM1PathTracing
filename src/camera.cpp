#include "camera.h"
#include <cmath>

vec3 camera::pitch(float x, float y, float z, float alpha){
    return vec3(x*cos(alpha)-y*sin(alpha), x*sin(alpha)+y*cos(alpha), z);
}
vec3 camera::yaw(float x, float y, float z, float beta){
    return vec3(x*cos(beta)+z*sin(beta), y , -x*sin(beta)+z*cos(beta));
}
vec3 camera::compute(vec3 v, float alpha, float beta){
    vec3 pitched = pitch(v.getX(), v.getY(), v.getZ(), alpha);
    vec3 yawed = yaw(v.getX(), v.getY(), v.getZ(), beta);
    vec3 res=yawed.cross(pitched);
    return res;
}