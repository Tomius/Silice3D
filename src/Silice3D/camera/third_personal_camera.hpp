// Copyright (c) Tamas Csala

#ifndef SILICE3D_CAMERA_THIRD_PERSONAL_CAMERA_HPP_
#define SILICE3D_CAMERA_THIRD_PERSONAL_CAMERA_HPP_

#include <Silice3D/camera/perspective_camera.hpp>

namespace Silice3D {

class ThirdPersonalCamera : public PerspectiveCamera {
 public:
  ThirdPersonalCamera(GameObject* parent,
                      double fov,
                      double z_near,
                      double z_far,
                      const glm::dvec3& position,
                      double mouse_sensitivity = 1.0,
                      double mouse_scroll_sensitivity = 1.0,
                      double min_dist_mod = 0.25,
                      double max_dist_mod = 4.00,
                      double base_distance = 0.0,
                      double dist_offset = 0.0);

  virtual ~ThirdPersonalCamera() {}

 private:
  // The target object's transform, that the camera is following
  Transform& target_;

  // We shouldn't interpolate at the first call.
  bool first_call_;

  // Private constant numbers
  const double initial_distance_, base_distance_, cos_max_pitch_angle_,
               mouse_sensitivity_, mouse_scroll_sensitivity_,
               min_dist_mod_, max_dist_mod_, dist_offset_;

  // For mouseScrolled interpolation
  double curr_dist_mod_, dest_dist_mod_;

  virtual void Update() override;
  virtual void MouseScrolled(double, double yoffset) override;
};  // ThirdPersonalCamera

}  // namespace Silice3D

#endif  // SILICE3D_CAMERA_THIRD_PERSONAL_CAMERA_HPP_
