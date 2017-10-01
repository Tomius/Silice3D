// Copyright (c) Tamas Csala

#include <Silice3D/mesh/mesh_object.hpp>
#include <Silice3D/common/optimizations.hpp>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/lighting/shadow.hpp>

namespace Silice3D {

MeshObject::MeshObject(GameObject* parent, const std::string& mesh_path,
                       const Transform& initial_transform,
                       const std::string& vertex_shader)
    : GameObject(parent, initial_transform)
    , renderer_(GetMeshRenderer(mesh_path, scene_->shader_manager(), scene_->mesh_cache(), vertex_shader))
{

}

MeshObject::~MeshObject() = default;

btCollisionShape* MeshObject::GetCollisionShape() {
  return renderer_->GetCollisionShape();
}

BoundingBox MeshObject::GetBoundingBox() const {
  return renderer_->GetBoundingBox(transform().matrix());
}

void MeshObject::Update() {
  if (Optimizations::kInstanceGrouping) {
    auto bbox = GetBoundingBox();
    const auto& cam = *scene()->camera();
    bool is_visible = !Optimizations::kFrustumCulling ||
                       bbox.CollidesWithFrustum(cam.frustum());
    if (is_visible) {
      renderer_->AddInstanceToRenderBatch(this);
    }

    // const auto& shadow_cam = *scene()->shadow_camera();
    // bool is_shadow_visible = !Optimizations::kFrustumCulling ||
    //                           bbox.CollidesWithFrustum(shadow_cam.frustum());
    bool is_shadow_visible = true;  // TODO
    if (is_shadow_visible) {
      renderer_->AddInstanceToShadowRenderBatch(this);
    }
  }
}

void MeshObject::Render() {
  if (!Optimizations::kInstanceGrouping) {
    const auto& cam = *scene()->camera();
    bool is_visible = !Optimizations::kFrustumCulling ||
                       GetBoundingBox().CollidesWithFrustum(cam.frustum());
    if (is_visible) {
      renderer_->AddInstanceToRenderBatch(this);
      renderer_->RenderBatch(scene());
      renderer_->ClearRenderBatch();
    }
  }
}

void MeshObject::ShadowRender(const ICamera& shadow_camera) {
  if (!Optimizations::kInstanceGrouping) {
    bool is_visible = !Optimizations::kFrustumCulling ||
                       GetBoundingBox().CollidesWithFrustum(shadow_camera.frustum());
    if (is_visible) {
      renderer_->AddInstanceToShadowRenderBatch(this);
      renderer_->ShadowRenderBatch(scene(), shadow_camera);
      renderer_->ClearShadowRenderBatch();
    }
  }
}

}   // namespace Silice3D
