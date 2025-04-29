#pragma once

#include "obj.h"
#include <random>
#include <omp.h>
#include <chrono>

// Génère un nombre aléatoire pour créer une direction légèrement différente pour chaque rayon
inline double randomDouble()
{
    static std::uniform_real_distribution<double> dist(0.0,1.0);
    static std::default_random_engine generator;
    return dist(generator);
}

// Générateur de nombres aléatoires rapide et léger (XorShift)
inline double fastRandomDouble(unsigned int& seed)
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return (seed * 2.3283064365386963e-10); // 1/2^32
}

class Lambertian:public Material
{
    public:
        Vec3 albedo;     // Réflectance diffuse
        Lambertian(const Vec3& a):albedo(a){}
        virtual bool scatter(const Ray& ray_in,const HitInfo& hit,Vec3& attenuation,Ray& scattered)const;
        // Assure que la direction de diffusion est dans l'hémisphère de la normale
        Vec3 randomInHemisphere(const Vec3& normal,unsigned int& seed)const;// Assure que la direction de diffusion est dans l'hémisphère de la normale
    private:
        // Génère un vecteur unitaire aléatoire pour simuler l'effet de diffusion aléatoire de la réflexion lambertienne
        Vec3 randomUnitVector(unsigned int& seed)const;
};