#include "scene.h"

Scene::Scene(const Camera& cam) : camera(cam) {}

Camera Scene::getCamera() const { 
    return camera; 
}

void Scene::addObject(Object* object) { 
    objects.push_back(object); 
}

std::vector<Object*> Scene::getObjects() const { 
    return objects; 
}

bool Scene::trace(const Ray& ray, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = 1e9;

    for (Object* object : objects) {
        if (object->intersect(ray, tempRec)) {
            if (tempRec.t < closestSoFar) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }
    }

    return hitAnything;
}

Scene::~Scene() {
    for (Object* object : objects) {
        delete object; 
    }
}
