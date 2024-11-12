/*#include "firstCollision.h"

int main(int argc,char** argv)
{
    //light origin and direction
    Vec3 origin(0,0,0);
    Vec3 direction(1,0,0);
    Ray ray(origin,direction);
    //creat objects in the scene
    Sphere sphere(1,Vec3(6,0,0));
    AABB box(Vec3(2,-1,-1),Vec3(3,1,1));
    //add objects to the scene
    std::vector<Object*> scene;
    scene.push_back(&sphere);
    scene.push_back(&box);
    //find the first collision of a ray with an object in the scene
    HitInfo hitInfo=findFirstCollision(ray, scene);
    if (hitInfo.hitObject)
    {
        std::cout<<"First collision at "<<hitInfo.intersection<<" and the distance is "<<hitInfo.distance<<std::endl;
    }
    else
        std::cout<<"No collision"<<std::endl;
    
    return 0;
}
*/