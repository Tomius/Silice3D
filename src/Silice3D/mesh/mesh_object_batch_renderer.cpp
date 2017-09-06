// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/mesh/mesh_object_batch_renderer.hpp>

namespace Silice3D {

MeshObjectBatchRenderer::MeshObjectBatchRenderer(GameObject* parent)
  : GameObject(parent) { }

void MeshObjectBatchRenderer::Update() {
  MeshRendererCache* cache = scene()->mesh_cache();
  for (auto& pair : *cache) {
    pair.second->ClearRenderBatch();
    pair.second->ClearShadowRenderBatch();
  }
}

void MeshObjectBatchRenderer::Render() {
  MeshRendererCache* cache = scene()->mesh_cache();
  for (auto& pair : *cache) {
    pair.second->RenderBatch(scene());
  }
}

void MeshObjectBatchRenderer::ShadowRender() {
  MeshRendererCache* cache = scene()->mesh_cache();
  for (auto& pair : *cache) {
    pair.second->ShadowRenderBatch(scene());
  }
}

}   // namespace Silice3D
