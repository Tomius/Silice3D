// Copyright (c), Tamas Csala

#ifndef SILICE3D_COLLISION_BOUNDING_BOX_HPP_
#define SILICE3D_COLLISION_BOUNDING_BOX_HPP_

#include <Silice3D/common/glm.hpp>
#include <Silice3D/collision/frustum.hpp>

namespace Silice3D {

class BoundingBox {
  glm::vec3 mins_;
  glm::vec3 maxes_;
 public:
  BoundingBox() = default;

  BoundingBox(const glm::vec3& mins, const glm::vec3& maxes)
      : mins_(mins), maxes_(maxes) {}

  glm::vec3 mins() const { return mins_; }
  glm::vec3 maxes() const { return maxes_; }
  glm::vec3 center() const { return (maxes_+mins_) / 2.0f; }
  glm::vec3 extent() const { return maxes_-mins_; }

  bool CollidesWithSphere(const glm::vec3& center, float radius) const;
  bool CollidesWithFrustum(const Frustum& frustum) const;
};

}


#endif
