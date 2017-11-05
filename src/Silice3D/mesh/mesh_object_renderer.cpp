// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/mesh/mesh_object_renderer.hpp>

namespace Silice3D {

MeshObjectRenderer::MeshObjectRenderer (const std::string& mesh_path,
                                        ShaderManager* shader_manager,
                                        const std::string& vertex_shader)
    : mesh_("src/resource/" + mesh_path, aiProcessPreset_TargetRealtime_Fast |
                                         aiProcess_FlipUVs |
                                         aiProcess_PreTransformVertices |
                                         aiProcess_Triangulate |
                                         aiProcess_CalcTangentSpace)
    , prog_data_(shader_manager, vertex_shader) {
  mesh_.setup();
  mesh_.setupDiffuseTextures(kDiffuseTextureSlot);
}

MeshObjectRenderer::ProgramData::ProgramData(ShaderManager* shader_manager,
                                             const std::string& vertex_shader)
    : basic_prog_(shader_manager->get(vertex_shader),
                  shader_manager->get("mesh.frag"))
    , shadow_recieve_prog_(shader_manager->get(vertex_shader),
                           shader_manager->get("mesh_shadow.frag"))
    , shadow_cast_prog_(shader_manager->get(vertex_shader),
                        shader_manager->get("shadow.frag"))

    , bp_uProjectionMatrix_(basic_prog_, "uProjectionMatrix")
    , bp_uCameraMatrix_(basic_prog_, "uCameraMatrix")
    , bp_uModelMatrix_(basic_prog_, "uModelMatrix")

    , srp_uProjectionMatrix_(shadow_recieve_prog_, "uProjectionMatrix")
    , srp_uCameraMatrix_(shadow_recieve_prog_, "uCameraMatrix")
    , srp_uModelMatrix_(shadow_recieve_prog_, "uModelMatrix")
    , srp_uShadowCP_(shadow_recieve_prog_, "uShadowCP")

    , scp_uProjectionMatrix_(shadow_cast_prog_, "uProjectionMatrix")
    , scp_uCameraMatrix_(shadow_cast_prog_, "uCameraMatrix")
    , scp_uModelMatrix_(shadow_cast_prog_, "uModelMatrix") {
  gl::Use(basic_prog_);
  gl::UniformSampler(basic_prog_, "uDiffuseTexture").set(kDiffuseTextureSlot);
  basic_prog_.validate();

  gl::Use(shadow_recieve_prog_);
  gl::UniformSampler(shadow_recieve_prog_, "uDiffuseTexture").set(kDiffuseTextureSlot);
  shadow_recieve_prog_.validate();
  gl::Unuse(shadow_recieve_prog_);
}

btCollisionShape* MeshObjectRenderer::GetCollisionShape() {
  if (!bt_shape_) {
    bt_triangles_ = make_unique<btTriangleIndexVertexArray>();
    bt_indices_ = mesh_.btTriangles(bt_triangles_.get());
    bt_shape_ = make_unique<btBvhTriangleMeshShape>(bt_triangles_.get(), true);
  }

  return bt_shape_.get();
}

void MeshObjectRenderer::AddInstanceToRenderBatch(const GameObject* game_object) {
  instance_transforms_.push_back(game_object->GetTransform().GetMatrix());
}

void MeshObjectRenderer::ClearRenderBatch() {
  instance_transforms_.clear();
}

void MeshObjectRenderer::RenderBatch(Scene* scene) {
  const auto& cam = *scene->GetCamera();

  if (recieve_shadows_) {
    gl::Use(prog_data_.shadow_recieve_prog_);
    prog_data_.shadow_recieve_prog_.Update();

    prog_data_.srp_uProjectionMatrix_ = cam.GetProjectionMatrix();
    prog_data_.srp_uCameraMatrix_ = cam.GetCameraMatrix();
  } else {
    gl::Use(prog_data_.basic_prog_);
    prog_data_.basic_prog_.Update();

    prog_data_.bp_uProjectionMatrix_ = cam.GetProjectionMatrix();
    prog_data_.bp_uCameraMatrix_ = cam.GetCameraMatrix();
  }

  mesh_.uploadModelMatrices(instance_transforms_);
  mesh_.render(instance_transforms_.size());
  gl::UnuseProgram();
}

void MeshObjectRenderer::AddInstanceToShadowRenderBatch(const GameObject* game_object) {
  shadow_instance_transforms_.push_back(game_object->GetTransform().GetMatrix());
}

void MeshObjectRenderer::ClearShadowRenderBatch() {
  shadow_instance_transforms_.clear();
}

void MeshObjectRenderer::ShadowRenderBatch(Scene* scene, const ICamera& shadow_camera) {
  if (cast_shadows_) {
    auto prog_user = gl::MakeTemporaryBind(prog_data_.shadow_cast_prog_);
    prog_data_.shadow_cast_prog_.Update();

    prog_data_.scp_uProjectionMatrix_ = shadow_camera.GetProjectionMatrix();
    prog_data_.scp_uCameraMatrix_ = shadow_camera.GetCameraMatrix();

    std::vector<glm::mat4> visibile_object_transforms;
    for (const glm::mat4& transform : shadow_instance_transforms_) {
      BoundingBox bbox = GetBoundingBox(transform);
      bool is_visible = bbox.CollidesWithFrustum(shadow_camera.GetFrustum());
      if (is_visible) {
        visibile_object_transforms.push_back(transform);
      }
    }
    mesh_.uploadModelMatrices(visibile_object_transforms);
    mesh_.render(visibile_object_transforms.size());
  }
}

size_t MeshObjectRenderer::GetTriangleCount() const {
  return instance_transforms_.size() * mesh_.triangleCount();
}

BoundingBox MeshObjectRenderer::GetBoundingBox(const glm::mat4& transform) const {
  return mesh_.boundingBox(transform);
}

}   // namespace Silice3D

Silice3D::MeshObjectRenderer* Silice3D::GetMeshRenderer(const std::string& str, ShaderManager* shader_manager,
                                                        std::map<std::string, std::unique_ptr<IMeshObjectRenderer>>* mesh_cache,
                                                        const std::string& vertex_shader) {
  auto iter = mesh_cache->find(str);
  if (iter == mesh_cache->end()) {
    MeshObjectRenderer* renderer = new MeshObjectRenderer(str, shader_manager, vertex_shader);
    (*mesh_cache)[str] = std::unique_ptr<IMeshObjectRenderer>{renderer};
    return renderer;
  } else {
    return dynamic_cast<MeshObjectRenderer*>(iter->second.get());
  }
}


