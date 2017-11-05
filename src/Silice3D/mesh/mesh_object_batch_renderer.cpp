// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/mesh/mesh_object_batch_renderer.hpp>

namespace Silice3D {

MeshObjectBatchRenderer::MeshObjectBatchRenderer(GameObject* parent)
  : GameObject(parent) { }

void MeshObjectBatchRenderer::Update() {
  MeshRendererCache* cache = GetScene()->GetMeshCache();
  for (auto& pair : *cache) {
    pair.second->ClearRenderBatch();
    pair.second->ClearShadowRenderBatch();
  }
}

void MeshObjectBatchRenderer::Render() {
  MeshRendererCache* cache = GetScene()->GetMeshCache();
  for (auto& pair : *cache) {
    pair.second->RenderBatch(GetScene());
  }
}

void MeshObjectBatchRenderer::ShadowRender(const ICamera& shadow_camera) {
  MeshRendererCache* cache = GetScene()->GetMeshCache();
  for (auto& pair : *cache) {
    pair.second->ShadowRenderBatch(GetScene(), shadow_camera);
  }
}

}   // namespace Silice3D
