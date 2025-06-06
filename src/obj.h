#pragma once

#include "ray.h"
#include "vec3.h"
#include "material.h"

// Classe de base pour tous les objets de la scène
// Définit l'interface commune pour l'intersection avec les rayons
class Object
{
    public:
        Material* material;   // Matériau de l'objet

        Object(Material* mat):material(mat){}
        virtual Vec3 getCenter()const=0;
        // Calcule l'intersection avec un rayon
        // Retourne true si intersection trouvée et met à jour les paramètres
        virtual bool intersect(const Ray& ray,double tmin,double tmax,Vec3& intersection,double& t,Vec3& normal)const=0;
        virtual ~Object()=default;
};