// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADOW_CASTER_HPP_
#define SILICE3D_SHADOW_CASTER_HPP_

#include <vector>
#include <glad/glad.h>
#include <Silice3D/common/glm.hpp>
#include <oglwrap/shader.h>
#include <oglwrap/uniform.h>
#include <oglwrap/framebuffer.h>

#include <Silice3D/core/game_object.hpp>

namespace Silice3D {

// TODO: Shadow caster for point lights
class ShadowCaster : public GameObject {
 public:
  ShadowCaster(GameObject* parent, size_t shadow_map_size, size_t cascades_count);

  gl::Texture2DArray& shadow_texture();
  const gl::Texture2DArray& shadow_texture() const;

  void FillShadowMap(Scene* scene);

  glm::mat4 GetProjectionMatrix(unsigned cascade_idx) const;
  glm::mat4 GetCameraMatrix(unsigned cascade_idx) const;

  size_t cascades_count() const;

 private:
  gl::Texture2DArray depth_tex_;
  std::vector<gl::Framebuffer> fbos_;

  size_t w_ = 0, h_ = 0, size_ = 0;
  std::vector<glm::vec4> target_bounding_spheres_;
  float z_near_ = 0.0f;
  float z_far_ = 0.0f;

  virtual void ScreenResized(size_t width, size_t height) override;
  virtual void Update() override;
};

}

#endif  // LOD_SHADOW_H_
