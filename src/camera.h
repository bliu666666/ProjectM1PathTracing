#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "vec3.h"

class camera {
    public:
        vec3 E;
        vec3 t = vec3(1, 0, 0);
        vec3 v = vec3(0, 1, 0);
        vec3 b = vec3(0, 0, 1);

        vec3 pitch(float x, float y, float z, float alpha);
        vec3 yaw(float x, float y, float z, float beta);
        vec3 compute(vec3 v, float alpha, float beta);

}; 


#endif