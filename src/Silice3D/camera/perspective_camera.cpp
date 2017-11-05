// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/camera/perspective_camera.hpp>

namespace Silice3D {

class CameraTransform : public Transform {
 public:
  // CameraTransform shouldn't inherit the parent's rotation,
  // like how a normal Transform does
  virtual quat GetRot() const override { return rot_; }
  virtual void SetRot(const quat& new_rot) override { rot_ = new_rot; }

  // CameraTransform has custom up and right vectors
  virtual vec3 GetUp() const override { return up_; }
  virtual void SetUp(const vec3& new_up) override {
    up_ = glm::normalize(new_up);
  }

  virtual vec3 GetRight() const override {
    return glm::normalize(glm::cross(GetForward(), GetUp()));
  }
  virtual void SetRight(const vec3& new_right) override {
    SetForward(glm::cross(GetUp(), new_right));
  }

 private:
  vec3 up_ = {0.0, 1.0, 0.0};
};

PerspectiveCamera::PerspectiveCamera(GameObject* parent, double fovy, double z_near, double z_far)
    : ICamera(parent, CameraTransform{}), fovy_(fovy), z_near_(z_near)
    , z_far_(z_far), width_(0), height_(0) {
  assert(fovy_ < M_PI);
}

void PerspectiveCamera::ScreenResized(size_t width, size_t height) {
  width_ = width;
  height_ = height;
}

void PerspectiveCamera::UpdateCache() {
  UpdateCameraMatrix();
  UpdateProjectionMatrix();
  UpdateFrustum();
}

void PerspectiveCamera::UpdateCameraMatrix() {
  const Transform& t = GetTransform();
  cam_mat_ = glm::lookAt(t.GetPos(), t.GetPos()+t.GetForward(), t.GetUp());
}

void PerspectiveCamera::UpdateProjectionMatrix() {
  proj_mat_ = glm::perspectiveFov<float>(fovy_, width_, height_, z_near_, z_far_);
}

}  // namespace Silice3D
