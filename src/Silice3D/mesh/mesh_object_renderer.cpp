// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/lighting/shadow.hpp>
#include <Silice3D/common/optimizations.hpp>
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
    , local_prog_cache_(shader_manager, vertex_shader)
    , prog_cache_(Optimizations::kSharedPrograms ?
                  GetProgramCacheEntry(shader_manager, vertex_shader) :
                  local_prog_cache_) {
  mesh_.setup();
  mesh_.setupDiffuseTextures(kDiffuseTextureSlot);
}

MeshObjectRenderer::ProgramCacheEntry::ProgramCacheEntry(ShaderManager* shader_manager,
                                                         const std::string& vertex_shader)
    : basic_prog_(shader_manager->get(vertex_shader),
                  shader_manager->get("mesh.frag"))
    , shadow_recieve_prog_(shader_manager->get(vertex_shader),
                           shader_manager->get("mesh_shadow.frag"))
    , shadow_cast_prog_(shader_manager->get(
               Optimizations::kAttribModelMat ? "shadow_attribute_model_mat.vert"
                                              : "shadow_uniform_model_mat.vert"),
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
  gl::UniformSampler(shadow_recieve_prog_, "uShadowMap").set(kShadowTextureSlot);
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

static std::vector<glm::mat4> ReorderTransforms(std::vector<const GameObject*>& instances,
                                                const ICamera& camera) {
  std::vector<glm::mat4> transforms;
  if (Optimizations::kDepthOrdering) {
    glm::vec3 camPos = camera.transform().pos();
    std::sort(instances.begin(), instances.end(), [camPos](const GameObject* a, const GameObject* b) -> bool {
      float a_dist = glm::length(a->transform().pos() - camPos);
      float b_dist = glm::length(b->transform().pos() - camPos);
      return Optimizations::kInverseDepthOrdering ? a_dist > b_dist : a_dist < b_dist;
    });
  }
  for (const GameObject* instance : instances) {
    transforms.push_back(instance->transform());
  }

  return transforms;
}

void MeshObjectRenderer::AddInstanceToRenderBatch(const GameObject* game_object) {
  if (Optimizations::kDelayedModelMatrixEvalutaion) {
    instances_.push_back(game_object);
  } else {
    instance_transforms_.push_back(game_object->transform().matrix());
  }
}

void MeshObjectRenderer::ClearRenderBatch() {
  if (Optimizations::kDelayedModelMatrixEvalutaion) {
    instances_.clear();
  } else {
    instance_transforms_.clear();
  }
}

void MeshObjectRenderer::RenderBatch(Scene* scene) {
  const auto& cam = *scene->camera();

  if (recieve_shadows_) {
    auto shadow_cam = scene->shadow_camera();

    gl::Use(prog_cache_.shadow_recieve_prog_);
    prog_cache_.shadow_recieve_prog_.update();

    prog_cache_.srp_uProjectionMatrix_ = cam.projectionMatrix();
    prog_cache_.srp_uCameraMatrix_ = cam.cameraMatrix();
    prog_cache_.srp_uShadowCP_ = shadow_cam->projectionMatrix() * shadow_cam->cameraMatrix();
  } else {
    gl::Use(prog_cache_.basic_prog_);
    prog_cache_.basic_prog_.update();

    prog_cache_.bp_uProjectionMatrix_ = cam.projectionMatrix();
    prog_cache_.bp_uCameraMatrix_ = cam.cameraMatrix();
  }

  if (Optimizations::kAttribModelMat) {
    if (Optimizations::kDelayedModelMatrixEvalutaion) {
      mesh_.uploadModelMatrices(ReorderTransforms(instances_, cam));
      mesh_.render(instances_.size());
    } else {
      mesh_.uploadModelMatrices(instance_transforms_);
      mesh_.render(instance_transforms_.size());
    }
  } else {
    for (glm::mat4& transform : instance_transforms_) {
      if (recieve_shadows_) {
        prog_cache_.srp_uModelMatrix_ = transform;
      } else {
        prog_cache_.bp_uModelMatrix_ = transform;
      }
      mesh_.render(1);
    }
  }
  gl::UnuseProgram();
}

void MeshObjectRenderer::AddInstanceToShadowRenderBatch(const GameObject* game_object) {
  if (Optimizations::kDelayedModelMatrixEvalutaion) {
    shadow_instances_.push_back(game_object);
  } else {
    shadow_instance_transforms_.push_back(game_object->transform().matrix());
  }
}

void MeshObjectRenderer::ClearShadowRenderBatch() {
  if (Optimizations::kDelayedModelMatrixEvalutaion) {
    shadow_instances_.clear();
  } else {
    shadow_instance_transforms_.clear();
  }
}

void MeshObjectRenderer::ShadowRenderBatch(Scene* scene) {
  if (cast_shadows_) {
    const auto& shadow_cam = *scene->shadow_camera();

    auto prog_user = gl::MakeTemporaryBind(prog_cache_.shadow_cast_prog_);
    prog_cache_.shadow_cast_prog_.update();

    prog_cache_.scp_uProjectionMatrix_ = shadow_cam.projectionMatrix();
    prog_cache_.scp_uCameraMatrix_ = shadow_cam.cameraMatrix();

    if (Optimizations::kAttribModelMat) {
      if (Optimizations::kDelayedModelMatrixEvalutaion) {
        mesh_.uploadModelMatrices(ReorderTransforms(shadow_instances_, shadow_cam));
        mesh_.render(shadow_instances_.size());
      } else {
        mesh_.uploadModelMatrices(shadow_instance_transforms_);
        mesh_.render(shadow_instance_transforms_.size());
      }
    } else {
      for (glm::mat4& transform : shadow_instance_transforms_) {
        prog_cache_.scp_uModelMatrix_ = transform;
        mesh_.render(1);
      }
    }
  }
}

size_t MeshObjectRenderer::GetTriangleCount() const {
  if (Optimizations::kDelayedModelMatrixEvalutaion) {
    return instances_.size() * mesh_.triangleCount();
  } else {
    return instance_transforms_.size() * mesh_.triangleCount();
  }
}

BoundingBox MeshObjectRenderer::GetBoundingBox(const glm::mat4& transform) const {
  return mesh_.boundingBox(transform);
}

MeshObjectRenderer::ProgramCacheEntry& MeshObjectRenderer::GetProgramCacheEntry(ShaderManager* shader_manager,
                                                                                const std::string& vertex_shader) {
  using ProgramCacheEntry = MeshObjectRenderer::ProgramCacheEntry;
  // TODO: cache per scene because of shader manager
  static std::map<std::string, std::unique_ptr<ProgramCacheEntry>> cache;
  auto iter = cache.find(vertex_shader);
  if (iter == cache.end()) {
    ProgramCacheEntry* cache_entry = new ProgramCacheEntry(shader_manager, vertex_shader);
    cache[vertex_shader] = std::unique_ptr<ProgramCacheEntry>{cache_entry};
    return *cache_entry;
  } else {
    return *iter->second.get();
  }
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


