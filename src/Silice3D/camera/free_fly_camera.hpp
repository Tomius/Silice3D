// Copyright (c) Tamas Csala

#ifndef SILICE3D_CAMERA_FREE_FLY_CAMERA_HPP_
#define SILICE3D_CAMERA_FREE_FLY_CAMERA_HPP_

#include <Silice3D/camera/perspective_camera.hpp>

namespace Silice3D {

class FreeFlyCamera : public PerspectiveCamera {
 public:
  FreeFlyCamera(GameObject* parent, double fov, double z_near,
                double z_far, const glm::dvec3& pos,
                const glm::dvec3& target = glm::dvec3{0.0},
                double speed_per_sec = 5.0f,
                double mouse_sensitivity = 5.0f);

  double speed_per_sec() const { return speed_per_sec_; }
  double mouse_sensitivity() const { return mouse_sensitivity_; }
  double cos_max_pitch_angle() const { return cos_max_pitch_angle_; }

  void set_speed_per_sec(double value) { speed_per_sec_ = value; }
  void set_mouse_sensitivity(double value) { mouse_sensitivity_ = value; }
  void set_cos_max_pitch_angle(double value) { cos_max_pitch_angle_ = value; }

 protected:
  bool first_call_;
  double speed_per_sec_, mouse_sensitivity_, cos_max_pitch_angle_;

 private:
  virtual void Update() override;
};

}  // namespace Silice3D

#endif  // SILICE3D_CAMERA_FREE_FLY_CAMERA_HPP_
