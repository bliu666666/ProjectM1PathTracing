#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <iostream>
#include "vec3.h"
#include "camera.h"

class viewport {
    public:
        float theta; //Field of view
        int k; //width
        int m; //height
        camera cam;
        vec3 C=cam.t.operator+(cam.E); //center
        

        float hx=2*tan(theta/2);
        float hy=hx*((m-1)/(k-1));
        vec3 qx=cam.b.operator*(hx/(k-1));
        vec3 qy=cam.v.operator*(hy/(m-1));

        vec3 initialPosition=C-cam.b.operator*(hx/2)-cam.v.operator*(hy/2);
        vec3 pixelPosition(vec3 initialPosition, int i, int j, vec3 qx, vec3 qy);


}; 


#endif