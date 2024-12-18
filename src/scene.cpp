#include "scene.h"

Scene::Scene() : camera() {}

void Scene::setCamera(const Camera& camera) { 
    this->camera = camera; 
}

Camera Scene::getCamera() const { 
    return camera; 
}

void Scene::addObject(Object* object) { 
    objects.push_back(object); 
}

std::vector<Object*> Scene::getObjects() const { 
    return objects; 
}

bool Scene::trace(const Ray& ray, double& t, Object*& hitObject) const {
    t = 1e9;
    hitObject = nullptr;

    for (Object* object : objects) {
        double tTemp;

        // Test des intersections
        if (object->intersect(ray, tTemp)) {
            if (tTemp > 1e-4 && tTemp < t) { 
                t = tTemp;   
                hitObject = object;
            }
        }
    }

    return hitObject != nullptr;
}

Scene::~Scene() {
    for (Object* object : objects) {
        delete object; 
    }
}
