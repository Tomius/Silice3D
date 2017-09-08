// Copyright (c) Tamas Csala

#ifndef SILICE3D_CAMERA_BULLET_FREE_FLY_CAMERA_HPP_
#define SILICE3D_CAMERA_BULLET_FREE_FLY_CAMERA_HPP_

#include <Silice3D/camera/free_fly_camera.hpp>
#include <Silice3D/physics/bullet_rigid_body.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

class BulletFreeFlyCamera : public FreeFlyCamera {
 public:
  BulletFreeFlyCamera(GameObject* parent, float fov, float z_near,
                      float z_far, const glm::vec3& pos,
                      const glm::vec3& target = glm::vec3{0.0},
                      float speed_per_sec = 5.0f,
                      float mouse_sensitivity = 1.0f);

 private:
  btRigidBody* bt_rigid_body_;

  virtual void Update() override;
};

}

#endif  // SILICE3D_CAMERA_BULLET_FREE_FLY_CAMERA_HPP_
