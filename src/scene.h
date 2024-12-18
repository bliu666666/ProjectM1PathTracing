#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "../shapes/object.h"
#include "camera.h"

class Scene {
private:
    Camera camera;
    std::vector<Object*> objects; // List of objects in the scene

public:
    // Constructor
    Scene();

    // Camera methods
    void setCamera(const Camera& camera);
    Camera getCamera() const;

    // Object methods
    void addObject(Object* object);
    std::vector<Object*> getObjects() const;

    // Ray tracing
    bool trace(const Ray& ray, double& t, Object*& hitObject) const;

    ~Scene();
};

#endif // SCENE_H
