#include "AABB.h"

bool AABB::intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const
{
    /*for (int i=0;i<3;i++)//traverse the three coordinate axes x, y, and z
    {
        double invD=1.0f/ray.direction[i];//calculate the inverse of the directional component of the light
        double t0=(min[i]-ray.origin[i])*invD;//T(l,i)=(l_i-e_i)/r_i
        double t1=(max[i]-ray.origin[i])*invD;//T(h,i) = (h_i-e_i)/r_i
        if (invD<1.0f)//if the light direction is reversed
            std::swap(t0,t1);
        //T(close) = max{T(close,0),T_(close,1),T_(close,2)}
        //T(far) = min{T(far,0),T(far,1),T(far,2)}
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
    return true;*/

    double t0,t1;
    double invD;
    Vec3 tempNormal;//Temporarily store normal vector

    //Check the x-axis
    invD=1.0f/ray.direction[0];//calculate the inverse of the directional component of the light
    t0=(min[0]-ray.origin[0])*invD;//T(l,0)=(l_0-e_0)/r_0
    t1=(max[0]-ray.origin[0])*invD;//T(h,0) = (h_0-e_0)/r_0
    if (invD<1.0f)//if the light direction is reversed
        std::swap(t0,t1);
    //T(close) = max{T(close,0),T_(close,1),T_(close,2)}
    //T(far) = min{T(far,0),T(far,1),T(far,2)}
    if (t0>tmin){
        tmin=t0;
        tempNormal=(invD<0.0)?Vec3(1,0,0):Vec3(-1,0,0);
    }
    tmax=std::min(tmax,t1);
    if (tmax<=tmin) 
        return false;
    
    //Check the y-axis
    invD=1.0f/ray.direction[1];//calculate the inverse of the directional component of the light
    t0=(min[1]-ray.origin[1])*invD;//T(l,1)=(l_1-e_1)/r_1
    t1=(max[1]-ray.origin[1])*invD;//T(h,1) = (h_1-e_1)/r_1
    if (invD<1.0f)//if the light direction is reversed
        std::swap(t0,t1);
    //T(close) = max{T(close,0),T_(close,1),T_(close,2)}
    //T(far) = min{T(far,0),T(far,1),T(far,2)}
    if (t0>tmin){
        tmin=t0;
        tempNormal=(invD<0.0)?Vec3(0,1,0):Vec3(0,-1,0);
    }
    tmax=std::min(tmax,t1);
    if (tmax<=tmin) 
        return false;
    
    //Check the z-axis
    invD=1.0f/ray.direction[2];//calculate the inverse of the directional component of the light
    t0=(min[2]-ray.origin[2])*invD;//T(l,2)=(l_2-e_2)/r_2
    t1=(max[2]-ray.origin[2])*invD;//T(h,2) = (h_2-e_2)/r_2
    if (invD<1.0f)//if the light direction is reversed
        std::swap(t0,t1);
    //T(close) = max{T(close,0),T_(close,1),T_(close,2)}
    //T(far) = min{T(far,0),T(far,1),T(far,2)}
    if (t0>tmin){
        tmin=t0;
        tempNormal=(invD<0.0)?Vec3(0,0,1):Vec3(0,0,-1);
    }
    tmax=std::min(tmax,t1);
    if (tmax<=tmin) 
        return false;
    
    //Update intersection information
    t=tmin;
    intersection=ray.at(t);
    normal=tempNormal;//Assign the final normal vector to the output parameter
    return true;
}

Vec3 AABB::getCenter()const
{
    return (min+max)/2;
}