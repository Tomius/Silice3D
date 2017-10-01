// Copyright (c) Tamas Csala

#ifndef SILICE3D_MESH_IMESH_OBJECT_RENDERER_HPP_
#define SILICE3D_MESH_IMESH_OBJECT_RENDERER_HPP_

#include <map>
#include <string>
#include <memory>

namespace Silice3D {

class Scene;

class IMeshObjectRenderer {
public:
  virtual void ClearRenderBatch() = 0;
  virtual void RenderBatch(Scene* scene) = 0;

  virtual void ClearShadowRenderBatch() = 0;
  virtual void ShadowRenderBatch(Scene* scene) = 0;

  virtual size_t GetTriangleCount() const = 0;

  virtual ~IMeshObjectRenderer();
};
using MeshRendererCache = std::map<std::string, std::unique_ptr<IMeshObjectRenderer>>;

}   // namespace Silice3D

#endif  // SILICE3D_MESH_IMESH_OBJECT_RENDERER_HPP_
