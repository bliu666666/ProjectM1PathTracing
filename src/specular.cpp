#include "specular.h"

Vec3 Specular::reflect(const Vec3& v,const Vec3& n)const
{
    // The law of reflection of mirror reflection
    return v-2*v.dot(n)*n;
}

bool Specular::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    Vec3 reflected=reflect(ray_in.direction.normalize(),hit.normal);
    scattered=Ray(hit.intersection,reflected);
    attenuation=albedo;
    return true;
}