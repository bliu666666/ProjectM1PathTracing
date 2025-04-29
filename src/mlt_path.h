#pragma once

#include "camera.h"
#include "lambertian.h"
#include <vector>
#include "firstCollision.h"
#include "emissive.h"

// Represent a vertex on the path: position, normal, material, direction, etc.
struct PathVertex {
    Vec3 position;              // Hit position
    Vec3 normal;                // Normal direction
    Vec3 incoming;             //Incident direction (current light direction)
    const Material* material;  // Material of the object that was hit
    Vec3 weight;               // The cumulative weight of the current path (throughput)
};

//Thread-safe random number generation
double randomDoubleThread(unsigned int& seed);

// Start from the ray, propagate in the scene, generate a path with a maximum of max_depth
std::vector<PathVertex> generatePath(const Ray& ray,const std::vector<Object*>& scene,int max_depth);

// Evaluate the final color value based on the path
Vec3 evaluatePath(const std::vector<PathVertex>& path);

// Path sampling and main loop (Metropolis sampling)
Vec3 metropolisRender(const std::vector<Object*>& scene,const Camera& camera,int max_depth,int num_iterations);