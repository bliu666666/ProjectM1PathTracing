#ifndef SCENE_H
#define SCENE_H

#include "../shapes/object.h"
#include "camera.h"
#include <vector>
#include "hitrecord.h"

class Scene {
private:
    std::vector<Object*> objects;
    Camera camera;

public:
    Scene(const Camera& cam);
    ~Scene();

    void addObject(Object* object);
    std::vector<Object*> getObjects() const;
    Camera getCamera() const;

    bool trace(const Ray& ray, HitRecord& rec) const;
};

#endif // SCENE_H
