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
  virtual glm::mat4 cameraMatrix() const = 0;
  virtual glm::mat4 projectionMatrix() const = 0;
  const Frustum& frustum() const { return frustum_; }

protected:
  void UpdateFrustum();

private:
  Frustum frustum_;
};

}

#endif  // SILICE3D_CAMERA_ICAMERA_HPP_
