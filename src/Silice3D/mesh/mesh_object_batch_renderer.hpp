// Copyright (c) Tamas Csala

#ifndef SILICE3D_MESH_MESH_OBJECT_BATCH_RENDERER_HPP_
#define SILICE3D_MESH_MESH_OBJECT_BATCH_RENDERER_HPP_

#include <Silice3D/mesh/mesh_object_renderer.hpp>

namespace Silice3D {

class MeshObjectBatchRenderer : public GameObject {
 public:
  MeshObjectBatchRenderer(GameObject* parent);

 private:
  virtual void Update() override;
  virtual void Render() override;
  virtual void RenderDepthOnly(const ICamera& camera) override;
};

}   // namespace Silice3D

#endif  // SILICE3D_MESH_MESH_OBJECT_BATCH_RENDERER_HPP_
