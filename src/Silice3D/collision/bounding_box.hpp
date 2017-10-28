// Copyright (c), Tamas Csala

#ifndef SILICE3D_COLLISION_BOUNDING_BOX_HPP_
#define SILICE3D_COLLISION_BOUNDING_BOX_HPP_

#include <Silice3D/common/glm.hpp>
#include <Silice3D/collision/frustum.hpp>

namespace Silice3D {

class BoundingBox {
 public:
  BoundingBox() = default;

  BoundingBox(const glm::dvec3& mins, const glm::dvec3& maxes)
      : mins_(mins), maxes_(maxes) {}

  glm::dvec3 GetMins() const { return mins_; }
  glm::dvec3 GetMaxes() const { return maxes_; }
  glm::dvec3 GetCenter() const { return (maxes_+mins_) / 2.0; }
  glm::dvec3 GetExtent() const { return maxes_-mins_; }

  bool CollidesWithSphere(const glm::dvec3& center, double radius) const;
  bool CollidesWithFrustum(const Frustum& frustum) const;

private:
  glm::dvec3 mins_ = {0.0, 0.0, 0.0};
  glm::dvec3 maxes_= {0.0, 0.0, 0.0};
};

} // namespace Silice3D


#endif
