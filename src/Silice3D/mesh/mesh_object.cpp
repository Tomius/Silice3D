// Copyright (c) Tamas Csala

#include <Silice3D/mesh/mesh_object.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

MeshObject::MeshObject(GameObject* parent, const std::string& mesh_path,
                       const Transform& initial_transform,
                       const std::string& vertex_shader)
    : GameObject(parent, initial_transform)
    , renderer_(GetMeshRenderer(mesh_path, GetScene()->GetShaderManager(),
                                GetScene()->GetMeshCache(), vertex_shader))
{

}

MeshObject::~MeshObject() = default;

btCollisionShape* MeshObject::GetCollisionShape() {
  return renderer_->GetCollisionShape();
}

BoundingBox MeshObject::GetBoundingBox() const {
  return renderer_->GetBoundingBox(GetTransform().GetMatrix());
}

void MeshObject::Update() {
  auto bbox = GetBoundingBox();
  const auto& cam = *GetScene()->GetCamera();
  bool is_visible = bbox.CollidesWithFrustum(cam.GetFrustum());
  if (is_visible) {
    renderer_->AddInstanceToRenderBatch(this);
  }

  renderer_->AddInstanceToRenderDepthOnlyBatch(this);
}

}   // namespace Silice3D
