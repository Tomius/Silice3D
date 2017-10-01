// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADOW_HPP_
#define SILICE3D_SHADOW_HPP_

#include <vector>
#include <glad/glad.h>
#include <oglwrap/shader.h>
#include <oglwrap/uniform.h>
#include <oglwrap/framebuffer.h>

#include <Silice3D/core/game_object.hpp>

namespace Silice3D {

class Shadow : public GameObject {
 public:
  Shadow(GameObject* parent, const glm::vec3& light_source_pos, int shadow_map_size);

  gl::Texture2DArray& shadow_texture();
  const gl::Texture2DArray& shadow_texture() const;

  void FillShadowMap(Scene* scene);

  glm::mat4 GetProjectionMatrix(unsigned cascade_idx) const;
  glm::mat4 GetCameraMatrix(unsigned cascade_idx) const;

  static constexpr const unsigned kCascadesCount = 4;

 private:
  gl::Texture2DArray depth_tex_;
  gl::Framebuffer fbo_[kCascadesCount];

  size_t w_, h_, size_;
  glm::vec3 light_source_pos_;
  glm::vec4 target_bounding_spheres_[kCascadesCount];

  virtual void ScreenResized(size_t width, size_t height) override;
  virtual void Update() override;
};

}

#endif  // LOD_SHADOW_H_
