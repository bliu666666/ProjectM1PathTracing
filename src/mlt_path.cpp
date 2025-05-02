#include "mlt_path.h"
#include <cmath>

double randomDoubleThread(unsigned int& seed) 
{
    static thread_local std::mt19937 generator; // Each thread is independent
    generator.seed(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    seed = generator(); // mix seed for next call
    return distribution(generator);
}

std::vector<PathVertex> generatePath(const Ray& ray, const std::vector<Object*>& scene, int max_depth) 
{
    std::vector<PathVertex> path;
    Ray currentRay = ray;
    // Energy of the current path (each channel is initialized to 1.0, representing white light)
    Vec3 currentWeight(1.0, 1.0, 1.0);
    for (int depth = 0; depth < max_depth; ++depth) {
        HitInfo hitInfo = findFirstCollision(currentRay, scene);
        if (!hitInfo.hitObject) break; // Terminate the path if there is no collision
        Ray scattered;
        Vec3 attenuation;

         // If the material defines scattering behavior (such as diffuse reflection, refraction, specular, etc.)
        if (hitInfo.hitObject->material->scatter(currentRay, hitInfo, attenuation, scattered)) {
            //Construct a path vertex and record the information of the current intersection
            PathVertex vertex{hitInfo.intersection, hitInfo.normal, currentRay.direction, hitInfo.hitObject->material, currentWeight};
            path.push_back(vertex);

            // Update current light and energy
            currentRay = scattered;
            currentWeight = currentWeight * attenuation;
        } else {
            // Non-scatterable materials (such as Emissive) also record the end point
            PathVertex vertex{hitInfo.intersection, hitInfo.normal, currentRay.direction, hitInfo.hitObject->material, currentWeight};
            path.push_back(vertex);
            break;
        }
    }
    return path;
}


// Support light source sampling to check whether the luminous material is hit in the path
Vec3 evaluatePath(const std::vector<PathVertex>& path) 
{
    if (path.empty()) return Vec3(0,0,0);
    Vec3 result(0,0,0);
    for (const auto& v : path) {
        const Emissive* light = dynamic_cast<const Emissive*>(v.material);
        if (light && (-v.incoming.normalize()).dot(v.normal.normalize()) > 0.0) {
            result += v.weight * light->emitted();
        }
    }

    // Add gamma correction
    return Vec3(std::sqrt(result.x), std::sqrt(result.y), std::sqrt(result.z));
}

Vec3 metropolisRender(const std::vector<Object*>& scene,const Camera& camera,int max_depth,int num_iterations,double u_init,double v_init,double pixel_w,double pixel_h) {
    // Initialize
    unsigned int seed_init = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    double u_curr = u_init;
    double v_curr = v_init;
    Vec3 currColor = evaluatePath(generatePath(camera.getRay(u_curr, v_curr), scene, max_depth));
    Vec3 accum(0,0,0);

    #pragma omp parallel
    {
        // Each thread generates an independent seed using its own ID and time
        unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) + omp_get_thread_num()*7919;
        Vec3 localAccum(0,0,0);
        double u_t = u_curr;
        double v_t = v_curr;
        Vec3 c_t = currColor;

        #pragma omp for schedule(dynamic)
        for (int i = 0; i < num_iterations; ++i) {
            double du = (randomDoubleThread(seed) - 0.5) * pixel_w; // Perturb the u coordinate
            double dv = (randomDoubleThread(seed) - 0.5) * pixel_h; // Perturb v coordinate
            double u1 = std::clamp(u_t + du, u_init, u_init + pixel_w);
            double v1 = std::clamp(v_t + dv, v_init, v_init + pixel_h);

             // Evaluate the new path’s radiance
            Vec3 newColor = evaluatePath(generatePath(camera.getRay(u1, v1), scene, max_depth));

            // Compute Metropolis acceptance probability
            double lumOld = c_t.x + c_t.y + c_t.z;
            double lumNew = newColor.x + newColor.y + newColor.z;
            double a = std::min(1.0, lumNew / (lumOld + 1e-6));

            // Accept or reject the proposal
            if (randomDoubleThread(seed) < a) {
                u_t = u1;
                v_t = v1;
                c_t = newColor;
            }
            localAccum += (c_t + newColor) * 0.5;
        }

        // Accumulate contribution
        #pragma omp critical
            accum += localAccum;
    }
    
    return accum / static_cast<double>(num_iterations);
}