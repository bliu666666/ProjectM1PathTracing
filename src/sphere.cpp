#include "sphere.h"

bool Sphere::intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const
{
    /*
        According to the mathematical definition, the intersection of the ray and the sphere 
        can be obtained by solving the quadratic equation 
        T^2+2(v*r)T+(v^2-R^2)=0.
    */

    Vec3 oc=ray.origin-center;
    double a=ray.direction.dot(ray.direction); //coefficient of the quadratic term
    double b=oc.dot(ray.direction);//coefficient of a linear term
    double c=oc.dot(oc)-r*r;//constant term
    double discriminant=b*b-a*c;

    if (discriminant>0) 
    {
        double sqrtDiscriminant=std::sqrt(discriminant);
        /*double temp=(-b-sqrtDiscriminant)/a;
        if (temp<tMax&&temp>tMin) 
        {
            t=temp;//distance
            intersection=ray.at(t);
            normal=(intersection-center).normalize();
            return true;
        }
        temp=(-b+sqrtDiscriminant)/a;
        if (temp<tMax&&temp>tMin) 
        {
            t=temp;
            intersection=ray.at(t);
            normal=(intersection-center).normalize();
            return true;
        }*/
        auto updateHitInfo=[&,tmin,tmax](double temp){
            if (temp<tmax&&temp>tmin)
            {
                t=temp;//distance
                intersection=ray.at(t);
                normal=(intersection-center).normalize();
                return true;
            }
            return false;
        };
        double temp1=(-b-sqrtDiscriminant)/a;
        double temp2=(-b+sqrtDiscriminant)/a;
        if (updateHitInfo(temp1)||updateHitInfo(temp2))
            return true;
    }
    return false;
}

Vec3 Sphere::getCenter()const
{
    return center;
}