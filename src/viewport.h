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
        vec3 C=cam.t+cam.E; //center
        

        float hx=2*tan(theta/2);
        float hy=hx*((m-1)/(k-1));
        vec3 qx=cam.b*(hx/(k-1));
        vec3 qy=cam.v*(hy/(m-1));

        
        vec3 pixelPosition(int i, int j, vec3 qx, vec3 qy);


}; 


#endif