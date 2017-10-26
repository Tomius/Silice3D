// Copyright (c) Tamas Csala

#include <vector>
#include <glad/glad.h>
#include <oglwrap/context.h>
#include <oglwrap/smart_enums.h>

#include <Silice3D/lighting/shadow_caster.hpp>
#include <Silice3D/core/scene.hpp>
#include <Silice3D/camera/perspective_camera.hpp>

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
  depth_tex_.maxAnisotropy();
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
  return glm::ortho<float>(-size, size, -size, size, 0, 2*z_far);
}

glm::mat4 ShadowCaster::GetCameraMatrix(unsigned cascade_idx) const {
  return glm::lookAt(
    glm::vec3(target_bounding_spheres_[cascade_idx]) + z_far * glm::normalize(glm::vec3(transform().pos())),
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
  ICamera* cam = scene()->camera();
  glm::vec3 cam_pos = cam->transform().pos();
  glm::vec3 cam_dir = cam->transform().forward();

  PerspectiveCamera* perspective_camera = dynamic_cast<PerspectiveCamera*>(cam);
  if (perspective_camera == nullptr) {
    throw std::runtime_error("ShadowCaster expected perspective camera");
  }

  z_near = perspective_camera->z_near();
  z_far = perspective_camera->z_far();

  float last_depth = z_near;
  for (int i = 0; i < fbos_.size(); ++i) {
    float max_depth = z_near * pow(z_far/z_near, (i+1.0f) / (fbos_.size()));
    target_bounding_spheres_[i] = glm::vec4{cam_pos + (last_depth+max_depth)/2.0f*cam_dir, max_depth-last_depth};
    last_depth = 0.8*max_depth;
  }
}

}