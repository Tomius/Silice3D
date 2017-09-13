// Copyright (c) Tamas Csala

#ifndef SILICE3D_DEBUG_DEBUG_TEXTURE_HPP_
#define SILICE3D_DEBUG_DEBUG_TEXTURE_HPP_

#include <glad/glad.h>
#include <oglwrap/oglwrap.h>
#include <oglwrap/shapes/rectangle_shape.h>

#include <Silice3D/shaders/shader_manager.hpp>

namespace Silice3D {

class DebugTexture {
  gl::RectangleShape rect_;
  ShaderProgram prog_;

public:
  DebugTexture(ShaderManager* shader_manager);

  void Render(const gl::Texture2D& tex);
};

} // namespace Silice3D

#endif  // DEBUG_TEXTURE_HPP_
