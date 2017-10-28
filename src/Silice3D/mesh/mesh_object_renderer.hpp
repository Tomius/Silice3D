// Copyright (c) Tamas Csala

#ifndef SILICE3D_MESH_MESH_OBJECT_RENDERER_HPP_
#define SILICE3D_MESH_MESH_OBJECT_RENDERER_HPP_

#include <Silice3D/common/oglwrap.hpp>
#include <Silice3D/common/make_unique.hpp>
#include <Silice3D/core/game_object.hpp>
#include <Silice3D/shaders/shader_manager.hpp>
#include <Silice3D/mesh/mesh_renderer.hpp>
#include <Silice3D/mesh/imesh_object_renderer.hpp>
#include <Silice3D/physics/bullet_rigid_body.hpp>

namespace Silice3D {

class ShaderManager;
class Scene;

class MeshObjectRenderer : public IMeshObjectRenderer {
public:
  MeshObjectRenderer (const std::string& mesh_path, ShaderManager* shader_manager,
                      const std::string& vertex_shader);

  btCollisionShape* GetCollisionShape();

  void AddInstanceToRenderBatch(const GameObject* game_object);
  virtual void ClearRenderBatch() override;
  virtual void RenderBatch(Scene* scene) override;

  void AddInstanceToShadowRenderBatch(const GameObject* game_object);
  virtual void ClearShadowRenderBatch() override;
  virtual void ShadowRenderBatch(Scene* scene, const ICamera& shadow_camera) override;

  BoundingBox GetBoundingBox(const glm::mat4& transform) const;

  ShaderProgram& basic_prog() { return prog_data_.basic_prog_; }
  ShaderProgram& shadow_recieve_prog() { return prog_data_.shadow_recieve_prog_; }
  ShaderProgram& shadow_cast_prog() { return prog_data_.shadow_cast_prog_; }

  void set_cast_shadows(bool value) { cast_shadows_ = value; }
  void set_recieve_shadows(bool value) { recieve_shadows_ = value; }

  virtual size_t GetTriangleCount() const override;

private:
  MeshRenderer mesh_;

  struct ProgramData {
    ShaderProgram basic_prog_;
    ShaderProgram shadow_recieve_prog_;
    ShaderProgram shadow_cast_prog_;

    // basic_prog uniforms
    gl::LazyUniform<glm::mat4> bp_uProjectionMatrix_, bp_uCameraMatrix_, bp_uModelMatrix_;

    // shadow_recieve_prog_ uniforms
    gl::LazyUniform<glm::mat4> srp_uProjectionMatrix_, srp_uCameraMatrix_, srp_uModelMatrix_, srp_uShadowCP_;

    // shadow_cast_prog_ uniforms
    gl::LazyUniform<glm::mat4> scp_uProjectionMatrix_, scp_uCameraMatrix_, scp_uModelMatrix_;

    ProgramData(ShaderManager* shader_manager,
                const std::string& vertex_shader);
  };

  ProgramData prog_data_;

  std::vector<int> bt_indices_;
  std::unique_ptr<btTriangleIndexVertexArray> bt_triangles_;
  std::unique_ptr<btCollisionShape> bt_shape_;

  std::vector<glm::mat4> instance_transforms_;
  std::vector<glm::mat4> shadow_instance_transforms_;

  bool cast_shadows_ = true;
  bool recieve_shadows_ = true;

  void EnsureModelMatrixBufferSize(size_t size);
  void SetupModelMatrixAttrib();
};

MeshObjectRenderer* GetMeshRenderer(const std::string& str, ShaderManager* shader_manager,
                                    std::map<std::string, std::unique_ptr<IMeshObjectRenderer>>* mesh_cache,
                                    const std::string& vertex_shader_path);

}

#endif
