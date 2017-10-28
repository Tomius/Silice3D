// Copyright (c) Tamas Csala

#ifndef SILICE3D_TRANSFORM_INL_HPP_
#define SILICE3D_TRANSFORM_INL_HPP_

namespace Silice3D {

template<typename T>
Transformation<T>::Transformation(Transformation<T>* parent /*= nullptr*/)
    : parent_(parent)
    , pos_(0, 0, 0)
    , scale_(1, 1, 1)
    , rot_(glm::quat_identity<T, glm::defaultp>()) { }

template<typename T>
Transformation<T>* Transformation<T>::GetParent() const {
  return parent_;
}

template<typename T>
void Transformation<T>::SetParent(Transformation<T>* parent) {
  parent_ = parent;
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetPos() const {
  if (parent_) {
    return vec3{parent_->GetLocalToWorldMatrix() * vec4{pos_, 1}};
  } else {
    return pos_;
  }
}

template<typename T>
void Transformation<T>::SetPos(const vec3& new_pos) {
  if (parent_) {
    pos_ = vec3{parent_->GetWorldToLocalMatrix() *
                vec4{new_pos - parent_->GetPos(), 0}};
  } else {
    pos_ = new_pos;
  }
}

template<typename T>
const glm::tvec3<T>& Transformation<T>::GetLocalPos() const {
  return pos_;
}

template<typename T>
void Transformation<T>::SetLocalPos(const vec3& new_pos) {
  pos_ = new_pos;
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetScale() const {
  if (parent_) {
    return mat3(parent_->GetLocalToWorldMatrix()) * scale_;
  } else {
    return scale_;
  }
}

template<typename T>
void Transformation<T>::SetScale(const vec3& new_scale) {
  if (parent_) {
    scale_ = mat3(parent_->GetWorldToLocalMatrix()) * new_scale;
  } else {
    scale_ = new_scale;
  }
}

template<typename T>
const glm::tvec3<T>& Transformation<T>::GetLocalScale() const {
  return scale_;
}

template<typename T>
void Transformation<T>::SetLocalScale(const vec3& new_scale) {
  scale_ = new_scale;
}

template<typename T>
glm::tquat<T> Transformation<T>::GetRot() const {
  if (parent_) {
    return parent_->GetRot() * rot_;
  } else {
    return rot_;
  }
}

template<typename T>
void Transformation<T>::SetRot(const quat& new_rot) {
  if (parent_) {
    rot_ = glm::inverse(parent_->GetRot()) * new_rot;
  } else {
    rot_ = new_rot;
  }
}

template<typename T>
const glm::tquat<T>& Transformation<T>::GetLocalRot() const {
  return rot_;
}

template<typename T>
void Transformation<T>::SetLocalRot(const quat& new_rot) {
  rot_ = new_rot;
}

template<typename T>
void Transformation<T>::SetRot(const vec3& local_space_vec, const vec3& world_space_vec) {
  vec3 local = glm::normalize(local_space_vec);
  vec3 world = glm::normalize(world_space_vec);

  // Rotate around the vector, that is orthogonal to both.
  vec3 axis = glm::cross(local, world);

  // If they are not parallel
  if (glm::length(axis) > Math::kEpsilon) {
    // Dot gives us the cosine of their angle
    T cosangle = glm::dot(local, world);
    // We need the angle in radians
    T angle = std::acos(cosangle);
    // Rotate with the calced values
    SetRot(glm::quat_cast(glm::rotate(angle, axis)));
  } else {
    // If they are parallel, we only have to care about the case
    // when they go the opposite direction
    if (glm::dot(local, world) < 0) {
      // Check if local is parallel to the X axis
      if (fabs(glm::dot(local, vec3(1, 0, 0))) > Math::kEpsilon) {
        // If not, we can use it, to generate the axis to rotate around
        vec3 axis = glm::cross(vec3(1, 0, 0), local);
        SetRot(glm::quat_cast(glm::rotate(T(M_PI), axis)));
      } else {
        // Else we can use the Y axis for the same purpose
        vec3 axis = glm::cross(vec3(0, 1, 0), local);
        SetRot(glm::quat_cast(glm::rotate(T(M_PI), axis)));
      }
    } else {
      SetRot(glm::quat_identity<T, glm::defaultp>());
    }
  }
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetLocalRotationAndScale() const {
  return scale_ * rot_;
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetRotationAndScale() const {
  if (parent_) {
    return parent_->GetRotationAndScale() * scale_ * rot_;
  } else {
    return scale_ * rot_;
  }
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetForward() const {
  return glm::normalize(GetRot() * vec3(0, 0, -1));
}

template<typename T>
void Transformation<T>::SetForward(const vec3& new_fwd) {
  SetRot(vec3(0, 0, -1), new_fwd);
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetUp() const {
  return glm::normalize(GetRot() * vec3(0, 1, 0));
}

template<typename T>
void Transformation<T>::SetUp(const vec3& new_up) {
   SetRot(vec3(0, 1, 0), new_up);
}

template<typename T>
glm::tvec3<T> Transformation<T>::GetRight() const {
  return glm::normalize(GetRot() * vec3(1, 0, 0));
}

template<typename T>
void Transformation<T>::SetRight(const vec3& new_right) {
  SetRot(vec3(1, 0, 0), new_right);
}

template<typename T>
glm::tmat4x4<T> Transformation<T>::GetWorldToLocalMatrix() const {
  return glm::inverse(GetLocalToWorldMatrix());
}

template<typename T>
glm::tmat4x4<T> Transformation<T>::GetLocalToWorldMatrix() const {
  mat4 local_transf = glm::scale(glm::mat4_cast(rot_), scale_);
  local_transf[3] = vec4(pos_, 1);

  if (parent_) {
    return parent_->GetLocalToWorldMatrix() * local_transf;
  } else {
    return local_transf;
  }
}

template<typename T>
glm::tmat4x4<T> Transformation<T>::GetMatrix() const {
  return GetLocalToWorldMatrix();
}

template<typename T>
glm::tmat4x4<T> Transformation<T>::GetInverseMatrix() const {
  return GetWorldToLocalMatrix();
}

template<typename T>
Transformation<T>::operator mat4() const {
  return GetLocalToWorldMatrix();
}

}  // namespace Silice3D

#endif
