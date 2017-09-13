// Copyright (c) Tamas Csala

#ifndef SILICE3D_COLLISION_PLANE_HPP_
#define SILICE3D_COLLISION_PLANE_HPP_

#include <cassert>
#include <Silice3D/common/glm.hpp>
#include <Silice3D/common/math.hpp>

namespace Silice3D {

struct Plane {
  glm::vec3 normal = {0.0, 0.0, 0.0};
  float dist = 0.0;

  Plane() = default;
  Plane(float nx, float ny, float nz, float dist);
  Plane(const glm::vec3& normal, float dist);

  void Normalize();
};

} // namespace Silice3D

#endif
