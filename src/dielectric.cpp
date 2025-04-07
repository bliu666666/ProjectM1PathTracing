#include "dielectric.h"

Vec3 Dielectric::reflect(const Vec3& v,const Vec3& n)const
{
    return v-2*v.dot(n)*n;
}

/*
    uv:Unit Vector
    n:Normal
    etai_over_etat:Refractive index ratio
*/
Vec3 Dielectric::refract(Vec3& uv,const Vec3& n,double etai_over_etat)const
{
    double cos_theta=fmin((-uv).dot(n), 1.0);
    Vec3 r_out_perp=etai_over_etat*(uv+cos_theta*n);
    Vec3 r_out_parallel=-sqrt(fabs(1.0-r_out_perp.length()*r_out_perp.length()))*n;
    return r_out_perp+r_out_parallel;
}

// Calculate reflection probability based on angle and refractive index
double Dielectric::schlick(double cosine,double ref_idx)const
{
    double r0=(1-ref_idx)/(1+ref_idx);
    r0=r0*r0;
    return r0+(1-r0)*pow((1-cosine),5);
}

bool Dielectric::scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const
{
    attenuation=Vec3(1.0,1.0,1.0);  // Transparent materials do not attenuate color

    // Determine the direction of refraction (into the air or out of the glass)
    double refraction_ratio=hit.front_face?(1.0/ref_idx):ref_idx;
    Vec3 unit_direction=ray_in.direction.normalize();

    // Calculate the cosine and sine of an angle
    double cos_theta=fmin((-unit_direction).dot(hit.normal),1.0);
    double sin_theta=sqrt(1.0-cos_theta*cos_theta);

    // Determine whether total reflection occurs
    bool cannot_refract=refraction_ratio*sin_theta>1.0;
    Vec3 direction;

    // Determine the reflection probability based on the Schlick approximation
    if (cannot_refract||schlick(cos_theta,ref_idx)>randomDouble())
        direction=reflect(unit_direction,hit.normal);
    else
        direction=refract(unit_direction,hit.normal,refraction_ratio);

    scattered=Ray(hit.intersection,direction);
    return true;
}