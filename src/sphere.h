#pragma once

#include "obj.h"

// Classe représentant une sphère
// Définie par son centre et son rayon
class Sphere:public Object
{
    public:
        double r;        // Rayon de la sphère
        Vec3 center;     // Centre de la sphère

        Sphere(Material* mat,const double r,const Vec3& center):Object(mat),r(r),center(center){}
        virtual Vec3 getCenter()const;
        // Vérifie si un rayon intersecte la sphère et calcule le point d'intersection
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const;
};