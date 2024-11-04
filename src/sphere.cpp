#include "sphere.h"

bool Sphere::intersect(const Ray& ray,double tMin,double tMax,Vec3& intersection,double& t)const
{
    /*
        According to the mathematical definition, the intersection of the ray and the sphere 
        can be obtained by solving the quadratic equation 
        T^2+2(v*r)T+(v^2-R^2)=0.
    */

    Vec3 oc=ray.origin-center;
    float a=ray.direction.dot(ray.direction); //coefficient of the quadratic term
    float b=oc.dot(ray.direction);//coefficient of a linear term
    float c=oc.dot(oc)-r*r;//constant term
    float discriminant=b*b-a*c;

    if (discriminant>0) 
    {
        float sqrtDiscriminant=std::sqrt(discriminant);
        float temp=(-b-sqrtDiscriminant)/a;
        if (temp<tMax&&temp>tMin) 
        {
            t=temp;//distance
            intersection=ray.at(t);
            return true;
        }
        temp=(-b+sqrtDiscriminant)/a;
        if (temp<tMax&&temp>tMin) 
        {
            t=temp;
            intersection=ray.at(t);
            return true;
        }
    }
    return false;
}