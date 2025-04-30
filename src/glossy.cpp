#include "glossy.h"

Vec3 Glossy::reflect(const Vec3& v,const Vec3& n)const
{
    return v-2*v.dot(n)*n;
}

Vec3 Glossy::randomInUnitSphere()const {
    while (true){
        Vec3 p(randomDouble()*2-1,randomDouble()*2-1,randomDouble()*2-1);
        if (p.length()*p.length()>=1) continue;
        return p;
    }
}

bool Glossy::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    Vec3 reflected=reflect(ray_in.direction.normalize(),hit.normal); // Calculate the ideal mirror reflection direction
    Vec3 scatter_dir=reflected+roughness*randomInUnitSphere(); // Add rough disturbance
    scattered=Ray(hit.intersection,scatter_dir);
    attenuation=albedo;
    return (scatter_dir.dot(hit.normal)>0); // Reflect only outward-facing light
}