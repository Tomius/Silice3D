// Copyright (c) Tamas Csala

#ifndef SILICE3D_CAMERA_PERSPECTIVE_CAMERA_HPP_
#define SILICE3D_CAMERA_PERSPECTIVE_CAMERA_HPP_

#include <Silice3D/camera/icamera.hpp>
#include <Silice3D/core/game_object.hpp>

namespace Silice3D {

class PerspectiveCamera : public ICamera {
 public:
  PerspectiveCamera(GameObject* parent, double fovy, double z_near, double z_far);
  virtual ~PerspectiveCamera() {}

  const glm::mat4& GetCameraMatrix() const override { return cam_mat_; }
  const glm::mat4& GetProjectionMatrix() const override { return proj_mat_; }

  virtual double GetFovx() const override { return fovy_*width_/height_; }
  virtual double GetFovy() const override { return fovy_; }
  virtual double GetZNear() const override { return z_near_; }
  virtual double GetZFar() const override { return z_far_; }

  void SetFovx(double fovx) { fovy_ = fovx*height_/width_; }
  void SetFovy(double fovy) { fovy_ = fovy; }
  void SetZNear(double z_near) { z_near_ = z_near; }
  void SetZFar(double z_far) { z_far_ = z_far; }

  virtual void ScreenResized(size_t width, size_t height) override;

 protected:
  // it must be called through Update()
  void UpdateCache();

 private:
  double fovy_, z_near_, z_far_, width_, height_;

  glm::mat4 cam_mat_{1.0}, proj_mat_{1.0};

  void UpdateCameraMatrix();
  void UpdateProjectionMatrix();
};

}  // namespace Silice3D

#endif  // SILICE3D_CAMERA_PERSPECTIVE_CAMERA_HPP_
