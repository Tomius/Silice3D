// Copyright (c) Tamas Csala

#include <Silice3D/camera/bullet_free_fly_camera.hpp>

namespace Silice3D {

BulletFreeFlyCamera::BulletFreeFlyCamera(GameObject* parent, double fov, double z_near,
                                         double z_far, const glm::dvec3& pos,
                                         const glm::dvec3& target /*= glm::dvec3{0.0}*/,
                                         double speed_per_sec /*= 5.0f*/,
                                         double mouse_sensitivity /*= 1.0f*/)
    : FreeFlyCamera(parent, fov, z_near, z_far, pos, target,
                    speed_per_sec, mouse_sensitivity) {
  double radius = 3.0f * z_near;
  btCollisionShape* shape = new btSphereShape(radius);
  auto rbody = AddComponent<BulletRigidBody>(
    1.0f, std::unique_ptr<btCollisionShape>{shape}, kColDynamic);
  BulletRigidBody::Restrains restrains;
  restrains.y_pos_lock = 1;
  restrains.manual_rot = 1;
  rbody->SetRestrains(restrains);
  bt_rigid_body_ = rbody->GetBtRigidBody();
  bt_rigid_body_->setGravity(btVector3{0, 0, 0});
  bt_rigid_body_->setActivationState(DISABLE_DEACTIVATION);
  bt_rigid_body_->setMassProps(0.1f, btVector3(0, 0, 0));
  bt_rigid_body_->setFriction(0.0f);
  bt_rigid_body_->setRestitution(0.0f);

  bt_rigid_body_->setCcdMotionThreshold(radius);
  bt_rigid_body_->setCcdSweptSphereRadius(radius/2.0f);
}

void BulletFreeFlyCamera::Update() {
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

  // Calculate the offset
  glm::dvec3 offset = {0.0, 0.0, 0.0};
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    offset += GetTransform().GetForward();
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    offset -= GetTransform().GetForward();
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    offset += GetTransform().GetRight();
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    offset -= GetTransform().GetRight();
  }
  offset.y = 0;
  if (length(offset) > Math::kEpsilon) {
    offset = normalize(offset);
  }
  offset *= speed_per_sec_;

  // Update the "position"
  bt_rigid_body_->setLinearVelocity(btVector3{offset.x, offset.y, offset.z});

  UpdateCache();
}

}
