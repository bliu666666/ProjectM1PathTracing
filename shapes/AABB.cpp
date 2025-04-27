#include "AABB.h"
#include <utility> 


AABB::AABB(Position minCorner, Position maxCorner, std::shared_ptr<Material> m)
    : Object((minCorner + maxCorner) * 0.5, std::move(m)), 
      minCorner(minCorner), 
      maxCorner(maxCorner) {}


AABB::AABB(Position center, double width, double height, double depth, std::shared_ptr<Material> m)
    : Object(center, std::move(m)) {
    double halfWidth = width * 0.5;
    double halfHeight = height * 0.5;
    double halfDepth = depth * 0.5;
    
    minCorner = Position(center.getX() - halfWidth, 
                        center.getY() - halfHeight, 
                        center.getZ() - halfDepth);
    maxCorner = Position(center.getX() + halfWidth, 
                        center.getY() + halfHeight, 
                        center.getZ() + halfDepth);
}

bool AABB::intersect(const Ray& ray, HitRecord& rec) const {
    double tMin = -1e9, tMax = 1e9;
    Position tempNormal(0, 0, 0); // <- Variable pour stocker la normale de la face d'entrée

    for (int i = 0; i < 3; i++) {
        double invD, t0, t1;
        double minCoord, maxCoord, originCoord, directionCoord;

        // Sélection des coordonnées selon l'axe
        if (i == 0) {
            minCoord = minCorner.getX(); maxCoord = maxCorner.getX();
            originCoord = ray.getOrigin().getX(); directionCoord = ray.getDirection().getX();
        } else if (i == 1) {
            minCoord = minCorner.getY(); maxCoord = maxCorner.getY();
            originCoord = ray.getOrigin().getY(); directionCoord = ray.getDirection().getY();
        } else {
            minCoord = minCorner.getZ(); maxCoord = maxCorner.getZ();
            originCoord = ray.getOrigin().getZ(); directionCoord = ray.getDirection().getZ();
        }

        if (std::abs(directionCoord) < 1e-8) {
            if (originCoord < minCoord || originCoord > maxCoord) return false;
            continue;
        }

        invD = 1.0 / directionCoord;
        t0 = (minCoord - originCoord) * invD;
        t1 = (maxCoord - originCoord) * invD;

        if (invD < 0.0) std::swap(t0, t1);

        if (t0 > tMin) {
            tMin = t0;
            // Met à jour la normale *temporaire* correspondante à cette entrée potentielle
            if (i == 0) tempNormal = (invD < 0.0) ? Position(1,0,0) : Position(-1,0,0);
            else if (i == 1) tempNormal = (invD < 0.0) ? Position(0,1,0) : Position(0,-1,0);
            else tempNormal = (invD < 0.0) ? Position(0,0,1) : Position(0,0,-1);
        }

        tMax = std::min(tMax, t1);
        if (tMax <= tMin) {
            return false; // Sortir dès que l'intervalle est invalide
        }
    }

    
    //S'assurer que l'intersection trouvée (tMin) est valide (devant le rayon)
    if (tMin < 1e-4) {
         return false; 
    }

    rec.t = tMin; // Utiliser tMin comme temps d'intersection
    rec.point = ray.at(rec.t);
    rec.setFaceNormal(ray, tempNormal); // Utiliser la normale temporaire calculée
    rec.material = material;

    return true;
}
