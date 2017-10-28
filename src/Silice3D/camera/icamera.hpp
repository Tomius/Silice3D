// Copyright (c) Tamas Csala

#ifndef SILICE3D_CAMERA_ICAMERA_HPP_
#define SILICE3D_CAMERA_ICAMERA_HPP_

#include <Silice3D/common/glm.hpp>
#include <Silice3D/core/game_object.hpp>
#include <Silice3D/collision/frustum.hpp>

namespace Silice3D {

class ICamera : public GameObject {
public:
  using GameObject::GameObject;

  virtual const glm::mat4& GetCameraMatrix() const = 0;
  virtual const glm::mat4& GetProjectionMatrix() const = 0;
  const Frustum& GetFrustum() const { return frustum_; }

  virtual double GetFovx() const = 0;
  virtual double GetFovy() const = 0;
  virtual double GetZNear() const = 0;
  virtual double GetZFar() const = 0;

protected:
  void UpdateFrustum();

private:
  Frustum frustum_;
};

}

#endif  // SILICE3D_CAMERA_ICAMERA_HPP_
