// Copyright (c) Tamas Csala

#ifndef SILICE3D_COLLISION_PLANE_HPP_
#define SILICE3D_COLLISION_PLANE_HPP_

#include <cassert>
#include <Silice3D/common/glm.hpp>
#include <Silice3D/common/math.hpp>

namespace Silice3D {

struct Plane {
  glm::dvec3 normal = {0.0, 0.0, 0.0};
  double dist = 0.0;

  Plane() = default;
  Plane(double nx, double ny, double nz, double dist);
  Plane(const glm::dvec3& normal, double dist);

  void Normalize();
};

} // namespace Silice3D

#endif
