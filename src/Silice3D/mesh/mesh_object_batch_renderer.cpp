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
    pair.second->ClearRenderDepthOnlyBatch();
  }
}

void MeshObjectBatchRenderer::Render() {
  MeshRendererCache* cache = GetScene()->GetMeshCache();
  for (auto& pair : *cache) {
    pair.second->RenderBatch(GetScene());
  }
}

void MeshObjectBatchRenderer::RenderDepthOnly(const ICamera& camera) {
  MeshRendererCache* cache = GetScene()->GetMeshCache();
  for (auto& pair : *cache) {
    pair.second->RenderDepthOnlyBatch(GetScene(), camera);
  }
}

}   // namespace Silice3D
