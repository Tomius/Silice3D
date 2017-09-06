// Copyright (c) Tamas Csala

#ifndef SILICE3D_DEBUG_DEBUG_SHAPE_HPP_
#define SILICE3D_DEBUG_DEBUG_SHAPE_HPP_

#include <glad/glad.h>
#include <oglwrap/oglwrap.h>
#include <oglwrap/shapes/cube_shape.h>
#include <oglwrap/shapes/sphere_shape.h>

#include <Silice3D/scene.hpp>
#include <Silice3D/game_object.hpp>
#include <Silice3D/shader_manager.hpp>

namespace Silice3D {
namespace debug {

template <typename Shape_t>
class DebugShape : public GameObject {
 public:
  DebugShape(GameObject* parent, const glm::vec3& color = glm::vec3());

  glm::vec3 color() { return color_; }
  void set_color(const glm::vec3& color) { color_ = color; }

 private:
  Shape_t shape_;
  ShaderProgram prog_;
  gl::LazyUniform<glm::mat4> uProjectionMatrix_, uCameraMatrix_, uModelMatrix_;
  gl::LazyUniform<glm::vec3> uColor_;

  glm::vec3 color_;

  virtual void Render() override;
};

using DebugCube = DebugShape<gl::CubeShape>;
using DebugSphere = DebugShape<gl::SphereShape>;

}  // namespcae debug
}  // namespace Silice3D

#include "./debug_shape-inl.hpp"

#endif
