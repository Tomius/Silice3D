// Copyright (c) Tamas Csala

#include "./bullet_rigid_body.hpp"

#include <Silice3D/core/scene.hpp>
#include <Silice3D/common/make_unique.hpp>

namespace Silice3D {


BulletRigidBody::BulletRigidBody(GameObject* parent, float mass,
                                 std::unique_ptr<btCollisionShape>&& shape,
                                 CollisionType collision_type)
    : GameObject(parent), shape_(std::move(shape)), up_to_date_(true) {
  Init(mass, shape_.get(), collision_type);
}

BulletRigidBody::BulletRigidBody(GameObject* parent, float mass,
                                 btCollisionShape* shape,
                                 CollisionType collision_type)
    : GameObject(parent), up_to_date_(true) {
  Init(mass, shape, collision_type);
}

BulletRigidBody::BulletRigidBody(GameObject* parent, float mass,
                                 btCollisionShape* shape,
                                 const glm::vec3& pos,
                                 CollisionType collision_type)
    : GameObject(parent), up_to_date_(true) {
  GetTransform().SetPos(pos);
  Init(mass, shape, collision_type);
}

BulletRigidBody::BulletRigidBody(GameObject* parent, float mass,
                                 std::unique_ptr<btCollisionShape>&& shape,
                                 const glm::vec3& pos,
                                 CollisionType collision_type)
    : GameObject(parent), shape_(std::move(shape)), up_to_date_(true) {
  GetTransform().SetPos(pos);
  Init(mass, shape_.get(), collision_type);
}

BulletRigidBody::BulletRigidBody(GameObject* parent, float mass, btCollisionShape* shape,
                                 const glm::vec3& pos, const glm::fquat& rot,
                                 CollisionType collision_type)
    : GameObject(parent), up_to_date_(true) {
  GetTransform().SetPos(pos);
  GetTransform().SetRot(rot);
  Init(mass, shape, collision_type);
}

BulletRigidBody::BulletRigidBody(GameObject* parent, float mass,
                                 std::unique_ptr<btCollisionShape>&& shape,
                                 const glm::vec3& pos, const glm::fquat& rot,
                                 CollisionType collision_type)
    : GameObject(parent), shape_(std::move(shape)), up_to_date_(true) {
  GetTransform().SetPos(pos);
  GetTransform().SetRot(rot);
  Init(mass, shape.get(), collision_type);
}

BulletRigidBody::Restrains::Restrains()
  : x_pos_lock{0}
  , y_pos_lock{0}
  , z_pos_lock{0}
  , manual_rot{0}
  , x_rot_lock{0}
  , y_rot_lock{0}
  , z_rot_lock{0}
{}

void BulletRigidBody::SetRestrains(Restrains value) {
  restrains_ = value;
  bt_rigid_body_->setLinearFactor(btVector3(
    1-value.x_pos_lock, 1-value.y_pos_lock, 1-value.z_pos_lock
  ));

  bt_rigid_body_->setAngularFactor(btVector3(
    1-value.x_rot_lock, 1-value.y_rot_lock, 1-value.z_rot_lock
  ));
}

void BulletRigidBody::Init(float mass, btCollisionShape* shape,
                           CollisionType collision_type) {
  btVector3 inertia(0, 0, 0);
  if (mass != 0.0f) {
    shape->calculateLocalInertia(mass, inertia);
  }
  btRigidBody::btRigidBodyConstructionInfo info{mass, this, shape, inertia};
  bt_rigid_body_ = make_unique<btRigidBody>(info);
  bt_rigid_body_->setUserPointer(parent_);
  if (mass == 0.0f) { bt_rigid_body_->setRestitution(1.0f); }
  GetScene()->GetBtWorld()->addRigidBody(bt_rigid_body_.get(), static_cast<int>(collision_type), CollidesWith(collision_type));
}

void BulletRigidBody::getWorldTransform(btTransform &t) const {
  if (up_to_date_) {
    const glm::vec3& pos = GetTransform().GetPos();
    t.setOrigin(btVector3{pos.x, pos.y, pos.z});
    const glm::fquat& rot = GetTransform().GetRot();
    t.setRotation(btQuaternion{rot.x, rot.y, rot.z, rot.w});
  } else {
    t = new_transform_;
  }
}

void BulletRigidBody::setWorldTransform(const btTransform &t) {
  new_transform_ = t;
  up_to_date_ = false;
}

void BulletRigidBody::UpdatePhysics() {
  if (!up_to_date_) {
    const btVector3& o = new_transform_.getOrigin();
    parent_->GetTransform().SetPos(glm::vec3{o.x(), o.y(), o.z()});
    if (!restrains_.manual_rot) {
      const btQuaternion& r = new_transform_.getRotation();
      parent_->GetTransform().SetRot(glm::quat(r.getW(), r.getX(),
                                            r.getY(), r.getZ()));
    }
    up_to_date_ = true;
  }
}

void BulletRigidBody::RemovedFromScene() {
  GetScene()->GetBtWorld()->removeCollisionObject(bt_rigid_body_.get());
}

}

