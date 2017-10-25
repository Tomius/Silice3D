// Copyright (c) Tamas Csala

#include <Silice3D/mesh/mesh_object.hpp>
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
  auto bbox = GetBoundingBox();
  const auto& cam = *scene()->camera();
  bool is_visible = bbox.CollidesWithFrustum(cam.frustum());
  if (is_visible) {
    renderer_->AddInstanceToRenderBatch(this);
  }

  // const auto& shadow_cam = *scene()->shadow_camera();
  // bool is_shadow_visible = bbox.CollidesWithFrustum(shadow_cam.frustum());
  bool is_shadow_visible = true;  // TODO
  if (is_shadow_visible) {
    renderer_->AddInstanceToShadowRenderBatch(this);
  }
}

}   // namespace Silice3D
