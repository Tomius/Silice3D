// Copyright (c) Tamas Csala

#include <Silice3D/camera/free_fly_camera.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

FreeFlyCamera::FreeFlyCamera(GameObject* parent, double fov, double z_near,
                             double z_far, const glm::dvec3& pos,
                             const glm::dvec3& target /*= glm::dvec3{0.0}*/,
                             double speed_per_sec /*= 5.0f*/,
                             double mouse_sensitivity /*= 5.0f*/)
    : PerspectiveCamera(parent, fov, z_near, z_far)
    , first_call_(true)
    , speed_per_sec_(speed_per_sec)
    , mouse_sensitivity_(mouse_sensitivity)
    , cos_max_pitch_angle_(0.98f) {
  GetTransform().SetPos(pos);
  GetTransform().SetForward(target - pos);
}

void FreeFlyCamera::Update() {
  glm::dvec2 cursor_pos;
  GLFWwindow* window = GetScene()->GetWindow();
  glfwGetCursorPos(window, &cursor_pos.x, &cursor_pos.y);
  static glm::dvec2 prev_cursor_pos;
  glm::dvec2 diff = cursor_pos - prev_cursor_pos;
  prev_cursor_pos = cursor_pos;

  // We get invalid diff values at the startup
  if (first_call_) {
    diff = glm::dvec2(0, 0);
    first_call_ = false;
  }

  const double dt = GetScene()->GetCameraTime().GetDeltaTime();

  // Mouse movement - update the coordinate system
  if (diff.x || diff.y) {
    double dx(diff.x * mouse_sensitivity_ / 10000);
    double dy(-diff.y * mouse_sensitivity_ / 10000);

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

  // Update the position
  double ds = dt * speed_per_sec_;
  glm::dvec3 local_pos = GetTransform().GetLocalPos();
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    local_pos += GetTransform().GetForward() * ds;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    local_pos -= GetTransform().GetForward() * ds;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    local_pos += GetTransform().GetRight() * ds;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    local_pos -= GetTransform().GetRight() * ds;
  }
  GetTransform().SetLocalPos(local_pos);

  PerspectiveCamera::UpdateCache();
}

}  // namespace Silice3D
