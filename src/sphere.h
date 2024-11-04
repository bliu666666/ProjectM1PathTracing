#include "obj.h"

class Sphere:public Object
{
    public:
        double r;
        Vec3 center;

        Sphere(const double r,const Vec3& center):r(r),center(center){}
        virtual bool intersect(const Ray& ray,double tMin,double tMax,Vec3& intersection,double& t)const;//check if the ray intersects the sphere
};