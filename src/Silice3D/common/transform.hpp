// Copyright (c) Tamas Csala

#ifndef SILICE3D_COMMON_TRANSFORM_HPP_
#define SILICE3D_COMMON_TRANSFORM_HPP_

#include <cmath>
#include <vector>
#include <algorithm>

#include <Silice3D/common/glm.hpp>
#include <Silice3D/common/math.hpp>

namespace Silice3D {

template<typename T>
class Transformation {
 public:
  using vec3 = glm::tvec3<T>;
  using vec4 = glm::tvec4<T>;
  using mat3 = glm::tmat3x3<T>;
  using mat4 = glm::tmat4x4<T>;
  using quat = glm::tquat<T>;

  Transformation(Transformation* parent = nullptr);
  virtual ~Transformation() = default;

  // ========== Getters ==============
  Transformation* GetParent() const;

  // ------ Position ------
  virtual vec3 GetPos() const;
  virtual const vec3& GetLocalPos() const;

  // ------ Scale ------
  virtual vec3 GetScale() const;
  virtual const vec3& GetLocalScale() const;

  // ------ Rotation ------
  virtual quat GetRot() const;
  virtual const quat& GetLocalRot() const;
  virtual vec3 GetLocalRotationAndScale() const;
  virtual vec3 GetRotationAndScale() const;
  virtual vec3 GetForward() const;
  virtual vec3 GetUp() const;
  virtual vec3 GetRight() const;

  // ------ Transformation matrix ------
  virtual mat4 GetWorldToLocalMatrix() const;
  virtual mat4 GetLocalToWorldMatrix() const;
  virtual mat4 GetMatrix() const;
  virtual mat4 GetInverseMatrix() const;
  operator mat4() const;

  // ========== Setters ==============
  void SetParent(Transformation* parent);

  // ------ Position ------
  virtual void SetPos(const vec3& new_pos);
  virtual void SetLocalPos(const vec3& new_pos);

  // ------ Scale ------
  virtual void SetScale(const vec3& new_scale);
  virtual void SetLocalScale(const vec3& new_scale);

  // ------ Rotation ------
  virtual void SetRot(const quat& new_rot);
  virtual void SetLocalRot(const quat& new_rot);
  // Sets the rotation, so that 'local_space_vec' in local space will be
  // equivalent to 'world_space_vec' in world space.
  virtual void SetRot(const vec3& local_space_vec, const vec3& world_space_vec);
  virtual void SetForward(const vec3& new_fwd);
  virtual void SetUp(const vec3& new_up);
  virtual void SetRight(const vec3& new_right);

protected:
  Transformation* parent_;
  vec3 pos_, scale_;
  quat rot_;
};

using Transform = Transformation<double>;

}  // namespace Silice3D

#include <Silice3D/common/transform-inl.hpp>

#endif
