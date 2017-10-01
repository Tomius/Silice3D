// Copyright (c) Tamas Csala

#ifndef SILICE3D_MESH_MESH_OBJECT_HPP_
#define SILICE3D_MESH_MESH_OBJECT_HPP_

#include <Silice3D/common/optimizations.hpp>
#include <Silice3D/mesh/mesh_object_renderer.hpp>

namespace Silice3D {

class MeshObject : public GameObject {
 public:
  MeshObject(GameObject* parent, const std::string& mesh_path,
             const Transform& initial_transform = Transform{},
             const std::string& vertex_shader =
                Optimizations::kAttribModelMat ? "mesh_attribute_model_mat.vert"
                                               : "mesh_uniform_model_mat.vert");
  virtual ~MeshObject();

  btCollisionShape* GetCollisionShape();
  BoundingBox GetBoundingBox() const;
  MeshObjectRenderer* GetRenderer() const { return renderer_; }

 protected:
  MeshObjectRenderer* renderer_;

  virtual void Update() override;
  virtual void Render() override;
  virtual void ShadowRender(const ICamera& shadow_camera) override;
};

}   // namespace Silice3D

#endif  // SILICE3D_MESH_MESH_OBJECT_HPP_
