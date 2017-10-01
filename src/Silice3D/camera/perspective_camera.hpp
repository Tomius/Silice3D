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

  glm::mat4 cameraMatrix() const override { return cam_mat_; }
  glm::mat4 projectionMatrix() const override { return proj_mat_; }

  double fovx() const { return fovy_*width_/height_;}
  void set_fovx(double fovx) { fovy_ = fovx*height_/width_; }
  double fovy() const { return fovy_;}
  void set_fovy(double fovy) { fovy_ = fovy; }
  double z_near() const { return z_near_;}
  void set_z_near(double z_near) { z_near_ = z_near; }
  double z_far() const { return z_far_;}
  void set_z_far(double z_far) { z_far_ = z_far; }

 protected:
  // it must be called through Update()
  void UpdateCache();

 private:
  double fovy_, z_near_, z_far_, width_, height_;

  glm::mat4 cam_mat_{1.0}, proj_mat_{1.0};

  void UpdateCameraMatrix();
  void UpdateProjectionMatrix();

  virtual void ScreenResized(size_t width, size_t height) override;
};

}  // namespace Silice3D

#endif  // SILICE3D_CAMERA_PERSPECTIVE_CAMERA_HPP_
