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

  double GetSpeedPerSec() const { return speed_per_sec_; }
  double GetMouseSensitivity() const { return mouse_sensitivity_; }
  double GetCosMaxPitchAngle() const { return cos_max_pitch_angle_; }

  void SetSpeedPerSec(double value) { speed_per_sec_ = value; }
  void SetMouseSensitivity(double value) { mouse_sensitivity_ = value; }
  void SetCosMaxPitchAngle(double value) { cos_max_pitch_angle_ = value; }

  virtual void Update() override;

 protected:
  bool first_call_;
  double speed_per_sec_, mouse_sensitivity_, cos_max_pitch_angle_;
};

}  // namespace Silice3D

#endif  // SILICE3D_CAMERA_FREE_FLY_CAMERA_HPP_
