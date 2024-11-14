#include "vec3.h"

Vec3::Vec3():x(0.0),y(0.0),z(0.0){}

Vec3::Vec3(double x,double y,double z)
{
    this->x=x;
    this->y=y;
    this->z=z;
}

Vec3 Vec3::operator+(const Vec3& v)const
{
    return Vec3(x+v.x,y+v.y,z+v.z);
}

Vec3 Vec3::operator-(const Vec3& v)const
{
    return Vec3(x-v.x,y-v.y,z-v.z);
}

Vec3 Vec3::operator*(const double t)const
{
    return Vec3(x*t,y*t,z*t);
}

Vec3 operator*(const double t,const Vec3& v)
{
    return Vec3(t*v.x,t*v.y,t*v.z);
}

Vec3 Vec3::operator*(const Vec3& v)const
{
    return Vec3(x*v.x,y*v.y,z*v.z);
} 

Vec3 Vec3::operator/(const double t)const
{
    return Vec3(x/t,y/t,z/t);
}

double Vec3::operator[](const int i)const
{
    if (i==0)
        return x;
    else if (i==1)
        return y;
    else
        return z;
}

double Vec3::dot(const Vec3& v)const
{
    return x*v.x+y*v.y+z*v.z;
}

Vec3 Vec3::cross(const Vec3& v)const
{
    return Vec3(y * v.z - z * v.y,z * v.x - x * v.z,x * v.y - y * v.x);
}

double Vec3::length()const
{
    return std::sqrt(x*x+y*y+z*z);
}

Vec3 Vec3::normalize()const
{
    return Vec3(x/length(),y/length(),z/length());
}

std::ostream& operator<<(std::ostream& os,const Vec3& v)
{
    os<<"("<<v.x<<","<<v.y<<","<<v.z<<")";
    return os;
}

bool Vec3::nearZero()const
{
    const double epsilon = 1e-8;
    return (std::fabs(x) < epsilon) && (std::fabs(y) < epsilon) && (std::fabs(z) < epsilon);
}