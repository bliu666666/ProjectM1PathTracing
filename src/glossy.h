#pragma once

#include "obj.h"
#include <random>
#include "lambertian.h"

// Glossy material class, simulates mirror reflection + blur control
class Glossy:public Material
{
    private:
        Vec3 reflect(const Vec3& v,const Vec3& n)const; // Calculate reflection direction
        Vec3 randomInUnitSphere() const; // Generate random vectors in the unit sphere for roughness control
    public:
        Vec3 albedo;        
        double roughness;   // roughness(0=pure specular,1=fully diffuse)
        Glossy(const Vec3& a,double r):albedo(a),roughness(r < 1 ? r : 1){} // Set the color and roughness (maximum is 1)
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const;
};