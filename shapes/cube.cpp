#include "cube.h"

Cube::Cube(Position position, double sideLength, std::vector<double> color)
    : Object(position, color), sideLength(sideLength) {}

bool Cube::intersect(const Ray& ray, double& t) const {
    // Initialisation
    double tMin = -1e9, tMax = 1e9;
    Position bounds[2] = {
        position - Position(sideLength / 2, sideLength / 2, sideLength / 2), // Min corner
        position + Position(sideLength / 2, sideLength / 2, sideLength / 2)  // Max corner
    };

    // Vérifier chaque axe (X, Y, Z)
    for (int i = 0; i < 3; i++) {
        double originCoord, directionCoord, minCoord, maxCoord;

        // Extraire les coordonnées selon l'axe (i = 0 pour X, i = 1 pour Y, i = 2 pour Z)
        if (i == 0) {
            originCoord = ray.getOrigin().getX();
            directionCoord = ray.getDirection().getX();
            minCoord = bounds[0].getX();
            maxCoord = bounds[1].getX();
        } else if (i == 1) {
            originCoord = ray.getOrigin().getY();
            directionCoord = ray.getDirection().getY();
            minCoord = bounds[0].getY();
            maxCoord = bounds[1].getY();
        } else {
            originCoord = ray.getOrigin().getZ();
            directionCoord = ray.getDirection().getZ();
            minCoord = bounds[0].getZ();
            maxCoord = bounds[1].getZ();
        }

        double invD = 1.0 / directionCoord; // Inverser la direction
        double t0 = (minCoord - originCoord) * invD;
        double t1 = (maxCoord - originCoord) * invD;

        if (invD < 0.0) {
            std::swap(t0, t1); // Inverser si la direction est négative
        }

        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);

        if (tMax <= tMin) {
            return false; // Pas d'intersection
        }
    }

    // Si on arrive ici, il y a une intersection
    t = tMin > 1e-4 ? tMin : tMax;
    return t >= 1e-4;
}
