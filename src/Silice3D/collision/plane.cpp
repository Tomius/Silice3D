// Copyright (c) Tamas Csala

#include "Silice3D/collision/plane.hpp"

namespace Silice3D {

Plane::Plane(float nx, float ny, float nz, float dist)
    : normal(nx, ny, nz), dist(dist) {
  Normalize();
}

Plane::Plane(const glm::vec3& normal, float dist)
    : normal(normal), dist(dist) {
  Normalize();
}

void Plane::Normalize() {
  double l = glm::length(normal);
  assert(l > Math::kEpsilon);
  if (l <= Math::kEpsilon) {
    normal = glm::dvec3{0.0, 0.0, 0.0};
    dist = 0.0;
  } else {
    normal /= l;
    dist /= l;
  }
}

} // namespace Silice3D
