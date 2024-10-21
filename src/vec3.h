#include <cmath>
#include <limits>

class Vec3
{
    public:
        double x,y,z;
        Vec3();
        Vec3(double x,double y,double z);
        Vec3 operator+(const Vec3& v)const;
        Vec3 operator-(const Vec3& v)const;
        Vec3 operator*(const double t)const;//Multiply a vector by a scalar
        friend Vec3 operator*(const double t,const Vec3& v);//Multiply a scalar by a vector
        Vec3 operator/(const double t)const;
        double operator[](const int i)const;
        double dot(const Vec3& v)const;//Vector dot product
        Vec3 cross(const Vec3& v)const;//Vector cross product
        double length()const;
        Vec3 normalize()const;
};