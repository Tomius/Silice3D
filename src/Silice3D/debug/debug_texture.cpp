// Copyright (c) Tamas Csala

#include <Silice3D/core/game_object.hpp>
#include <Silice3D/debug/debug_texture.hpp>

namespace Silice3D {

DebugTexture::DebugTexture(ShaderManager* shader_manager)
    : rect_({gl::RectangleShape::kPosition, gl::RectangleShape::kTexCoord})
    , prog_(shader_manager->GetShader("debug_texture.vert"),
            shader_manager->GetShader("debug_texture.frag")) {

  gl::Use(prog_);
  gl::UniformSampler(prog_, "uTex") = kDiffuseTextureSlot;
  (prog_ | "aPosition").bindLocation(rect_.kPosition);
  (prog_ | "aTexCoord").bindLocation(rect_.kTexCoord);
  prog_.validate();
  gl::Unuse(prog_);
}

void DebugTexture::Render(const gl::Texture2D& tex) {
  gl::Use(prog_);
  gl::BindToTexUnit(tex, kDiffuseTextureSlot);

  gl::TemporarySet capabilies{{{gl::kCullFace, false},
                               {gl::kDepthTest, false}}};

  rect_.render();
  gl::Unbind(tex);
  gl::Unuse(prog_);
}

} // namespace Silice3D
