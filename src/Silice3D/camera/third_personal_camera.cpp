// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/camera/third_personal_camera.hpp>

namespace Silice3D {

ThirdPersonalCamera::ThirdPersonalCamera(GameObject* parent,
                                         double fov,
                                         double z_near,
                                         double z_far,
                                         const glm::dvec3& position,
                                         double mouse_sensitivity /*= 1.0*/,
                                         double mouse_scroll_sensitivity /*= 1.0*/,
                                         double min_dist_mod /*= 0.25*/,
                                         double max_dist_mod /*= 4.00*/,
                                         double base_distance /*= 0.0*/,
                                         double dist_offset /*= 0.0*/)
    : PerspectiveCamera(parent, fov, z_near, z_far)
    , target_(parent->GetTransform())
    , first_call_(true)
    , initial_distance_(glm::length(target_.GetPos() - position) - dist_offset)
    , base_distance_(base_distance == 0.0 ? initial_distance_ : base_distance)
    , cos_max_pitch_angle_(0.999)
    , mouse_sensitivity_(mouse_sensitivity)
    , mouse_scroll_sensitivity_(mouse_scroll_sensitivity)
    , min_dist_mod_(min_dist_mod)
    , max_dist_mod_(max_dist_mod)
    , dist_offset_(dist_offset)
    , curr_dist_mod_(initial_distance_ / base_distance_)
    , dest_dist_mod_(curr_dist_mod_){
  GetTransform().SetPos(position);
  GetTransform().SetForward(target_.GetPos() - position);
}

void ThirdPersonalCamera::Update() {
  static glm::dvec2 prev_cursor_pos;
  glm::dvec2 cursor_pos;
  GLFWwindow* window = GetScene()->GetWindow();
  glfwGetCursorPos(window, &cursor_pos.x, &cursor_pos.y);
  glm::dvec2 diff = cursor_pos - prev_cursor_pos;
  prev_cursor_pos = cursor_pos;

  // We get invalid diff values at the startup
  if (first_call_ && (diff.x != 0 || diff.y != 0)) {
    diff = glm::dvec2(0, 0);
    first_call_ = false;
  }

  const double dt = GetScene()->GetCameraTime().GetDeltaTime();

  // Mouse movement - update the coordinate system
  if (diff.x || diff.y) {
    double mouse_sensitivity = mouse_sensitivity_ * curr_dist_mod_ / 1000;
    double dx(diff.x * mouse_sensitivity);
    double dy(-diff.y * mouse_sensitivity);

    // If we are looking up / down, we don't want to be able
    // to rotate to the other side
    double dot_up_fwd = glm::dot(GetTransform().GetUp(), GetTransform().GetForward());
    if (dot_up_fwd > cos_max_pitch_angle_ && dy > 0) {
      dy = 0;
    }
    if (dot_up_fwd < -cos_max_pitch_angle_ && dy < 0) {
      dy = 0;
    }

    GetTransform().SetForward(GetTransform().GetForward() +
                              GetTransform().GetRight()*dx +
                              GetTransform().GetUp()*dy);
  }

  double dist_diff_mod = dest_dist_mod_ - curr_dist_mod_;
  if (fabs(dist_diff_mod) > dt * mouse_scroll_sensitivity_) {
    curr_dist_mod_ *= dist_diff_mod > 0 ?
      (1 + dt * mouse_scroll_sensitivity_) :
      (1 - dt * mouse_scroll_sensitivity_);
  }

  // Update the position
  glm::dvec3 tpos(target_.GetPos()), fwd(GetTransform().GetForward());
  fwd = GetTransform().GetForward();
  double dist = curr_dist_mod_*base_distance_ + dist_offset_;
  glm::dvec3 pos = tpos - fwd*dist;
  GetTransform().SetPos(pos);

  PerspectiveCamera::UpdateCache();
}

void ThirdPersonalCamera::MouseScrolled(double, double yoffset) {
  dest_dist_mod_ *= 1 + (-yoffset) * 0.1 * mouse_scroll_sensitivity_;
  if (dest_dist_mod_ < min_dist_mod_) {
    dest_dist_mod_ = min_dist_mod_;
  } else if (dest_dist_mod_ > max_dist_mod_) {
    dest_dist_mod_ = max_dist_mod_;
  }
}

}  // namespace Silice3D
