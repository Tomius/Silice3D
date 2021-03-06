// Copyright (c) Tamas Csala

#ifndef SILICE3D_DEBUG_DEBUG_SHAPE_INL_HPP_
#define SILICE3D_DEBUG_DEBUG_SHAPE_INL_HPP_

#include <Silice3D/debug/debug_shape.hpp>

namespace Silice3D {

template<typename Shape_t>
DebugShape<Shape_t>::DebugShape(GameObject* parent, const glm::vec3& color)
      : GameObject(parent)
      , shape_{{Shape_t::kPosition, Shape_t::kNormal}}
      , prog_{GetScene()->GetShaderManager()->get("Silice3D/debug_shape.vert"),
              GetScene()->GetShaderManager()->get("Silice3D/debug_shape.frag")}
      , uProjectionMatrix_{prog_, "uProjectionMatrix"}
      , uCameraMatrix_{prog_, "uCameraMatrix"}
      , uModelMatrix_{prog_, "uModelMatrix"}
      , uColor_{prog_, "uColor"}
      , color_(color) {
  gl::Use(prog_);
  (prog_ | "aPosition").bindLocation(shape_.kPosition);
  (prog_ | "aNormal").bindLocation(shape_.kNormal);
  gl::Unuse(prog_);
}

template<typename Shape_t>
void DebugShape<Shape_t>::Render() {
  gl::Use(prog_);
  const auto& cam = *GetScene()->GetCamera();
  uCameraMatrix_.set(cam.GetCameraMatrix());
  uProjectionMatrix_.set(cam.GetProjectionMatrix());
  uModelMatrix_.set(GetTransform().GetMatrix());
  uColor_.set(color_);

  gl::FrontFace(shape_.faceWinding());
  gl::TemporaryEnable cullface{gl::kCullFace};
  shape_.render();
  gl::Unuse(prog_);
}

}  // namespace Silice3D

#endif
