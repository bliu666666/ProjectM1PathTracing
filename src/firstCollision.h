#pragma once

#include <vector>
#include <limits>
#include "sphere.h"
#include "AABB.h"
#include "hitInfo.h"

HitInfo findFirstCollision(const Ray& ray,const std::vector<Object*>& scene);