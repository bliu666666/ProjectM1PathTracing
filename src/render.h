#pragma once

#include "firstCollision.h"
#include "camera.h"
#include "../ppm_writer/ppm_writer.c"
#include "lambertian.h"

/*
    Calculates the color based on the intersection of the ray and the object. 
    If the ray intersects the object, the color is generated based on the normal vector of the intersection point; 
    otherwise, the background color is displayed.
*/
Vec3 calculateColor(const Ray& ray, const std::vector<Object*>& scene,int depth)
{
    if (depth <= 0) {
        return Vec3(0,0,0); //Exceed recursion depth, return black
    }
    HitInfo hitInfo=findFirstCollision(ray,scene);
    if (hitInfo.hitObject)
    {
        Ray scattered;
        Vec3 attenuation;
        //If the material's scatter function returns true, continue tracing the path
        if (hitInfo.hitObject->material->scatter(ray,hitInfo,attenuation,scattered))
        {
            return attenuation*calculateColor(scattered, scene, depth - 1);
        }
        return Vec3(0,0,0);//If the material does not scatter, returns black.
    }
    //Background color gradient
    Vec3 unitDirection=ray.direction.normalize();
    double t=(unitDirection.y+1.0)/2;
    return (1.0-t)*Vec3(1.0,1.0,1.0)+t*Vec3(0.5,0.7,1.0);
}

//rendering function
void render(double width,double height,const std::vector<Object*>& scene,char* outputPath,const Vec3& origin,const Vec3& lookat,
            const Vec3& v_up, double v_fov,int samples_per_pixel,int max_depth)
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
            Vec3 color(0,0,0);
            for (int s=0;s<samples_per_pixel;++s) {
                double u=(j+randomDouble())/width;
                double v=(i+randomDouble())/height;
                Ray ray=camera.getRay(u, v);
                color=color+calculateColor(ray,scene,max_depth);
            }
            color=color/static_cast<double>(samples_per_pixel);//take the average of the samples

            //map color values ​​to the range [0,255]
            img[3*(i*static_cast<int>(width)+j)+0]=sqrt(color.x);//Gamma Correction
            img[3*(i*static_cast<int>(width)+j)+1]=sqrt(color.y);
            img[3*(i*static_cast<int>(width)+j)+2]=sqrt(color.z);
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
    //assign Materials for Cornell Box
    Lambertian* red_wall = new Lambertian(Vec3(0.65, 0.05, 0.05));    // Rouge
    Lambertian* green_wall = new Lambertian(Vec3(0.12, 0.45, 0.15));  // Vert
    Lambertian* white_wall = new Lambertian(Vec3(0.73, 0.73, 0.73));  // Blanc
    Lambertian* red_material = new Lambertian(Vec3(0.8,0.3,0.3));

    std::vector<Object*> scene;
    
    // Back wall
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.0, -4.0), Vec3(2.0, 2.0, -3.99)));
    // Left wall (green)
    scene.push_back(new AABB(green_wall, Vec3(-2.01, -2.0, -4.0), Vec3(-2.0, 2.0, 0.0)));
    // Right wall (red)
    scene.push_back(new AABB(red_wall, Vec3(2.0, -2.0, -4.0), Vec3(2.01, 2.0, 0.0)));
    // Floor
    scene.push_back(new AABB(white_wall, Vec3(-2.0, -2.01, -4.0), Vec3(2.0, -2.0, 0.0)));
    // Ceiling
    scene.push_back(new AABB(white_wall, Vec3(-2.0, 2.0, -4.0), Vec3(2.0, 2.01, 0.0)));

    // Add a sphere inside for interest
    scene.push_back(new Sphere(red_material, 0.5, Vec3(0.0, -1.5, -2.0)));

    return scene;
}