// Copyright (c) Tamas Csala

#include <vector>
#include <glad/glad.h>
#include <oglwrap/context.h>
#include <oglwrap/smart_enums.h>

#include <Silice3D/lighting/shadow_caster.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

ShadowCaster::ShadowCaster(GameObject* parent, size_t shadow_map_size, size_t cascades_count)
    : GameObject(parent)
    , fbos_(cascades_count)
    , w_(0), h_(0)
    , size_(shadow_map_size)
    , target_bounding_spheres_(cascades_count) {
  assert(dynamic_cast<DirectionalLightSource*>(parent) != nullptr);
  gl::Bind(depth_tex_);
  depth_tex_.upload(static_cast<gl::enums::PixelDataInternalFormat>(GL_DEPTH_COMPONENT16),
                    size_, size_, cascades_count, gl::kDepthComponent, gl::kFloat, nullptr);
  depth_tex_.minFilter(gl::kLinear);
  depth_tex_.magFilter(gl::kLinear);
  depth_tex_.wrapS(gl::kClampToBorder);
  depth_tex_.wrapT(gl::kClampToBorder);
  depth_tex_.borderColor(glm::vec4(1.0f));
  depth_tex_.compareFunc(gl::kLequal);
  depth_tex_.compareMode(gl::kCompareRefToTexture);
  gl::Unbind(depth_tex_);

  // Setup the FBOs
  for (int i = 0; i < fbos_.size(); ++i) {
    gl::Bind(fbos_[i]);
    fbos_[i].attachTextureLayer(gl::kDepthAttachment, depth_tex_, 0, i);
    gl::DrawBuffer(gl::kNone);
    gl::ReadBuffer(gl::kNone);
    fbos_[i].validate();
    gl::Unbind(fbos_[i]);
  }

  depth_tex_.makeBindless();
  depth_tex_.makeResident();
}

void ShadowCaster::ScreenResized(size_t width, size_t height) {
  w_ = width;
  h_ = height;
}

glm::mat4 ShadowCaster::GetProjectionMatrix(unsigned cascade_idx) const {
  float size = target_bounding_spheres_[cascade_idx].w;
  return glm::ortho<float>(-size, size, -size, size, 0, 2*size);
}

glm::mat4 ShadowCaster::GetCameraMatrix(unsigned cascade_idx) const {
  return glm::lookAt(
    glm::vec3(target_bounding_spheres_[cascade_idx]) +
        glm::normalize(glm::vec3(transform().pos())) * target_bounding_spheres_[cascade_idx].w,
    glm::vec3(target_bounding_spheres_[cascade_idx]),
    glm::vec3(0, 1, 0));
}

gl::Texture2DArray& ShadowCaster::shadow_texture() {
  return depth_tex_;
}

const gl::Texture2DArray& ShadowCaster::shadow_texture() const {
  return depth_tex_;
}

class ShadowCasterCamera : public ICamera {
public:
  ShadowCasterCamera (GameObject* parent,
                      Transform transform,
                      const glm::mat4& projection_matrix,
                      const glm::mat4& camera_matrix)
      : ICamera(parent, transform)
      , projection_matrix_ (projection_matrix)
      , camera_matrix_ (camera_matrix)
  {
    UpdateFrustum();
  }

private:
  glm::mat4 projection_matrix_;
  glm::mat4 camera_matrix_;

  virtual glm::mat4 projectionMatrix() const override {
    return projection_matrix_;
  }

  virtual glm::mat4 cameraMatrix() const override {
    return camera_matrix_;
  }
};

void ShadowCaster::FillShadowMap(Scene* scene) {
  for (int i = 0; i < fbos_.size(); ++i) {
    gl::Bind(fbos_[i]);
    gl::Clear().Color().Depth();
    gl::Viewport(0, 0, size_, size_);

    ShadowCasterCamera shadowCamera (this, transform(), GetProjectionMatrix(i), GetCameraMatrix(i));
    scene->ShadowRenderAll(shadowCamera);

    gl::Unbind(fbos_[i]);
    gl::Viewport(0, 0, w_, h_);
  }
}

size_t ShadowCaster::cascades_count() const {
  return fbos_.size();
}

void ShadowCaster::Update() {
  auto cam = scene()->camera();
  auto mvp = cam->projectionMatrix() * cam->cameraMatrix();

  for (int i = 0; i < fbos_.size(); ++i) {
    float z_limit = pow((i+1.0)/fbos_.size(), 0.1) * 0.998;

    glm::vec4 points[] {
      /* near plane corners */
      {-1, -1, -z_limit, 1},
      {+1, -1, -z_limit, 1},
      {-1, +1, -z_limit, 1},
      {+1, +1, -z_limit, 1},

      /* "far" plane corners */
      {-1, -1, z_limit, 1},
      {+1, -1, z_limit, 1},
      {-1, +1, z_limit, 1},
      {+1, +1, z_limit, 1}
    };

    float zero = 0.0f;  // This is needed to bypass a visual c++ compile error
    float infty = 1.0f / zero;
    glm::vec3 mins{infty, infty, infty}, maxes{-infty, -infty, -infty};

    for (glm::vec4& point : points){
      point = glm::inverse(mvp) * point;
      point.x /= point.w;
      point.y /= point.w;
      point.z /= point.w;

      mins = glm::min(glm::vec3(point), mins);
      maxes = glm::max(glm::vec3(point), maxes);
    }

    target_bounding_spheres_[i] = glm::vec4{(maxes + mins) / 2.0f, glm::length(maxes - mins) * sqrt(2) / 2.0};
  }
}

}
