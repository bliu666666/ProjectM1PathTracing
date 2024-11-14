#include "AABB.h"

bool AABB::intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const
{
    for (int i=0;i<3;i++)//traverse the three coordinate axes x, y, and z
    {
        double invD=1.0f/ray.direction[i];//calculate the inverse of the directional component of the light
        double t0=(min[i]-ray.origin[i])*invD;//T(l,i)=(l_i-e_i)/r_i
        double t1=(max[i]-ray.origin[i])*invD;//T(h,i) = (h_i-e_i)/r_i
        if (invD<1.0f)//if the light direction is reversed
            std::swap(t0,t1);
        /*
        T(close) = max{T(close,0),T_(close,1),T_(close,2)}
        T(far) = min{T(far,0),T(far,1),T(far,2)}
        */
        if (t0>tmin)
        {
            tmin=t0;
            //set the normal vector to point to the face aligned with this axis
            if (i==0)
                normal=(invD<0.0)?Vec3(1,0,0):Vec3(-1,0,0);
            else if (i==1)
                normal=(invD<0.0)?Vec3(0,1,0):Vec3(0,-1,0);
            else
                normal=(invD<0.0)?Vec3(0,0,1):Vec3(0,0,-1);
        }
        tmax=t1<tmax?t1:tmax;
        //check the intersection condition:T(close)<T(far)
        if (tmax<=tmin)
            return false;
    }
    t=tmin;
    intersection=ray.at(t);
    return true;
}

Vec3 AABB::getCenter()const
{
    return (min+max)/2;
}