// Copyright (c) Tamas Csala

#ifndef SILICE3D_COLLISION_SPHERE_HPP_
#define SILICE3D_COLLISION_SPHERE_HPP_

#include <Silice3D/common/glm.hpp>
#include <Silice3D/collision/frustum.hpp>

namespace Silice3D {

class Sphere {
 public:
  Sphere() = default;
  Sphere(glm::vec3 const& center, float radius)
      : center_(center), radius_(radius) {}

  glm::vec3 center() const { return center_; }
  float radius() const { return radius_; }

  virtual bool CollidesWithSphere(const Sphere& sphere) const;
  virtual bool CollidesWithFrustum(const Frustum& frustum) const;

 private:
  glm::vec3 center_ = {0.0, 0.0, 0.0};
  float radius_ = 0.0;
};

} // namespace Silice3D


#endif
