#include "mlt_path.h"
#include <cmath>

double randomDoubleThread(unsigned int& seed)
{
    static thread_local std::mt19937 generator(seed);  // Each thread is independent
    static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

std::vector<PathVertex> generatePath(const Ray& ray,const std::vector<Object*>& scene,int max_depth) 
{
    std::vector<PathVertex> path;

    Ray currentRay=ray;

    // Energy of the current path (each channel is initialized to 1.0, representing white light)
    Vec3 currentWeight(1.0,1.0,1.0);

    for (int depth=0;depth < max_depth;++depth) {
        HitInfo hitInfo=findFirstCollision(currentRay,scene);
        if (!hitInfo.hitObject) break;  // Terminate the path if there is no collision

        Ray scattered;
        Vec3 attenuation;

        // If the material defines scattering behavior (such as diffuse reflection, refraction, specular, etc.)
        if (hitInfo.hitObject->material->scatter(currentRay,hitInfo,attenuation,scattered)) 
        {

            //Construct a path vertex and record the information of the current intersection
            PathVertex vertex;
            vertex.position=hitInfo.intersection;     
            vertex.normal=hitInfo.normal;            
            vertex.incoming=currentRay.direction;      
            vertex.material=hitInfo.hitObject->material; 
            vertex.weight=currentWeight;               

            path.push_back(vertex);

            // Update current light and energy
            currentRay=scattered;
            currentWeight=currentWeight*attenuation; // Record the attenuation of each channel
        } 
        else 
        {
            // Non-scatterable materials (such as Emissive) also record the end point
            PathVertex vertex;
            vertex.position=hitInfo.intersection;
            vertex.normal=hitInfo.normal;
            vertex.incoming=currentRay.direction;
            vertex.material=hitInfo.hitObject->material;
            vertex.weight=currentWeight;
            path.push_back(vertex);
            break; // Material does not support scattering, path terminated
        }
    }

    return path;
}

// Support light source sampling to check whether the luminous material is hit in the path
Vec3 evaluatePath(const std::vector<PathVertex>& path)
{
    if (path.empty()) return Vec3(0,0,0);

    Vec3 result(0,0,0);
    for (const auto& v:path) 
    {
        const Emissive* light=dynamic_cast<const Emissive*>(v.material);
        if (light&&(-v.incoming.normalize()).dot(v.normal.normalize())>0.0)
        {
            Vec3 contribution=v.weight*light->emitted();
            result+=contribution;
        }
    }

    // Add gamma correction
    return Vec3(std::sqrt(result.x),std::sqrt(result.y),std::sqrt(result.z));
}

Vec3 metropolisRender(const std::vector<Object*>& scene,const Camera& camera,int max_depth,int num_iterations) 
{
    Vec3 accumulatedColor(0,0,0);

    // 1. Initialize the path (generate an initial path with random light)
    unsigned int seed_init=std::chrono::system_clock::now().time_since_epoch().count();
    double u0=randomDoubleThread(seed_init);
    double v0=randomDoubleThread(seed_init);
    Ray initialRay=camera.getRay(u0,v0);
    std::vector<PathVertex> currentPath=generatePath(initialRay,scene,max_depth);
    Vec3 currentColor=evaluatePath(currentPath);

    #pragma omp parallel
    {
        // Each thread generates an independent seed using its own ID and time
        unsigned int thread_seed=std::chrono::system_clock::now().time_since_epoch().count()+omp_get_thread_num()*7919;
        Vec3 localAccum(0,0,0);

        #pragma omp for schedule(dynamic) 
        for (int i=0;i<num_iterations;++i)
        {
            // 2. Make a slight disturbance to the current path to generate a new path
            double u1=std::fmod(u0+(randomDoubleThread(thread_seed)-0.5)*0.05+1.0,1.0); // Perturb the u coordinate
            double v1=std::fmod(v0+(randomDoubleThread(thread_seed)-0.5)*0.05+1.0,1.0); // Perturb v coordinate
            Ray mutatedRay=camera.getRay(u1,v1);

            std::vector<PathVertex> newPath=generatePath(mutatedRay,scene,max_depth);
            Vec3 newColor=evaluatePath(newPath);

            // 3. Calculate the acceptance probability a = min(1, I_new / I_old)
            double currentLuminance=currentColor.x+currentColor.y+currentColor.z;
            double newLuminance=newColor.x+newColor.y+newColor.z;
            double a=std::min(1.0,newLuminance/(currentLuminance+1e-6));

            if (randomDoubleThread(thread_seed)<a)
            {
                // Accept the new path
                u0=u1;
                v0=v1;
                currentPath=newPath;
                currentColor=newColor;
            }

            localAccum+=(currentColor+newColor)*0.5;
        }

        // 4. Accumulate contribution (add currentColor regardless of whether the new path is accepted)
        #pragma omp critical
        {
            accumulatedColor+=localAccum;
        }
    }

    return accumulatedColor/static_cast<double>(num_iterations);
}