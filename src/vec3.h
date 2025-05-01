#pragma once

#include <iostream>
#include <cmath>
#include <limits>
#include "cuda_compat.h"

class Vec3
{
    public:
        double x,y,z;
        
        CUDA_HOST_DEVICE Vec3();
        CUDA_HOST_DEVICE Vec3(double x,double y,double z);
        CUDA_HOST_DEVICE Vec3 operator+(const Vec3& v)const;
        CUDA_HOST_DEVICE Vec3 operator+=(const Vec3& v);
        CUDA_HOST_DEVICE Vec3 operator-(const Vec3& v)const;
        CUDA_HOST_DEVICE Vec3 operator*(const double t)const;//Multiply a vector by a scalar
        CUDA_HOST_DEVICE friend Vec3 operator*(const double t,const Vec3& v);//Multiply a scalar by a vector
        CUDA_HOST_DEVICE Vec3 operator*(const Vec3& v)const;
        CUDA_HOST_DEVICE Vec3 operator/(const double t)const;
        CUDA_HOST_DEVICE double operator[](const int i)const;
        CUDA_HOST_DEVICE Vec3 operator-();//Take the opposite number
        CUDA_HOST_DEVICE double dot(const Vec3& v)const;//Vector dot product
        CUDA_HOST_DEVICE Vec3 cross(const Vec3& v)const;//Vector cross product
        CUDA_HOST_DEVICE double length()const;
        CUDA_HOST_DEVICE Vec3 normalize()const;
        CUDA_HOST friend std::ostream& operator<<(std::ostream& os,const Vec3& v);
        CUDA_HOST_DEVICE bool nearZero()const;//check whether a vector is close to zero
};

