// Copyright (c), Tamas Csala

#include <Silice3D/collision/bounding_box.hpp>

namespace Silice3D {

bool BoundingBox::CollidesWithSphere(const glm::dvec3& center, double radius) const {
  double dmin = 0;
  for (int i = 0; i < 3; ++i) {
    if (center[i] < mins_[i]) {
      dmin += Math::Sqr(center[i] - mins_[i]);
    } else if (center[i] > maxes_[i]) {
      dmin += Math::Sqr(center[i] - maxes_[i]);
    }
  }
  return dmin <= Math::Sqr(radius);
}

bool BoundingBox::CollidesWithFrustum(const Frustum& frustum) const {
  glm::dvec3 center = GetCenter();
  glm::dvec3 extent = GetExtent();

  for (int i = 0; i < 6; ++i) {
    const Plane& plane = frustum.planes[i];

    double d = glm::dot(center, plane.normal);
    double r = glm::dot(extent, glm::abs(plane.normal));

    if (d + r < -plane.dist) {
      return false;
    }
  }
  return true;
}

}

