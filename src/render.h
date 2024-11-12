#pragma once

#include "firstCollision.h"
#include "camera.h"
#include "../ppm_writer/ppm_writer.c"

/*
    Calculates the color based on the intersection of the ray and the object. 
    If the ray intersects the object, the color is generated based on the normal vector of the intersection point; 
    otherwise, the background color is displayed.
*/
Vec3 calculateColor(const Ray& ray, const std::vector<Object*>& scene)
{
    HitInfo hitInfo=findFirstCollision(ray,scene);
    if (hitInfo.hitObject)
    {
        //Generate simple colors using normal vectors (map normal vectors to RGB values ​​in [0, 1])
        Vec3 normal=(hitInfo.intersection-hitInfo.hitObject->getCenter()).normalize();
        return (normal+Vec3(1.0, 1.0, 1.0))/2;
    }
    //Background color gradient
    Vec3 unitDirection=ray.direction.normalize();
    double t=(unitDirection.y+1.0)/2;
    return (1.0-t)*Vec3(1.0,1.0,1.0)+t*Vec3(0.5,0.7,1.0);
}

//rendering function
void render(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
            const Vec3& v_up, double v_fov)
{
    double aspect=width/height;
    //use user-specified parameters so that the user can customize the camera viewing angle
    Camera camera(origin,lookat,v_up,v_fov,aspect);
    double *img=new double[static_cast<int>(width)*static_cast<int>(height)*3];
    //traverse each pixel, calculate the color and store it
    for (int i=height-1;i>=0;--i)
    {
        for (int j=0;j<width;++j)
        {
            double u=double(j)/width;
            double v=double(i)/height;
            Ray ray=camera.getRay(u,v);
            Vec3 color=calculateColor(ray,scene);

            img[3*(i * static_cast<int>(width)+j)+0]=color.x;
            img[3*(i * static_cast<int>(width)+j)+1]=color.y;
            img[3*(i * static_cast<int>(width)+j)+2]=color.z;
        }
    }
    unsigned char* rgbImg =img2rgb(width,height,img);
    writePPM(outputPath,width,height,rgbImg);
    delete []img;
    free(rgbImg);
}

//create scene
std::vector<Object*> createScene()
{
    std::vector<Object*> scene;
    scene.push_back(new Sphere(0.5,Vec3(0.0,0.0,-1.0)));
    scene.push_back(new Sphere(100.0,Vec3(0.0,-100.5,-1.0)));//as ground
    scene.push_back(new AABB(Vec3(-1.0,-0.5,-2.0),Vec3(1.0,0.5,-1.5)));
    return scene;
}