// Copyright (c) Tamas Csala

#include <glad/glad.h>
#include <oglwrap/oglwrap.h>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/core/game_engine.hpp>
#include <Silice3D/lighting/shadow_caster.hpp>

namespace Silice3D {

Scene::Scene(GameEngine* engine)
    : GameObject(nullptr)
    , camera_(nullptr)
    , engine_(engine)
    , physics_thread_should_quit_(false)
    , physics_thread_{[this](){
      while (true) {
        physics_can_run_.WaitOne();
        if (physics_thread_should_quit_) { return; }
        UpdatePhysicsInBackgroundThread();
        physics_finished_.Set();
      }
    }} {
  set_scene(this);

  shader_manager()->get("lighting.frag")->SetUpdateFunc([this](const gl::Program& prog) {
    static constexpr const size_t kMaxLightCount = 32;

    size_t current_light_index = 0;
    for (DirectionalLightSource* light : directional_light_sources_) {
      if (current_light_index >= kMaxLightCount) {
        break;
      }

      std::string uniform_name = "uDirectionalLights[" + std::to_string(current_light_index++) + "]";
      gl::Uniform<glm::vec3>(prog, uniform_name + ".direction") = light->transform().GetPos();
      gl::Uniform<glm::vec3>(prog, uniform_name + ".color") = light->color();
      ShadowCaster* shadow_caster = light->shadow_caster();
      if (shadow_caster == nullptr) {
        gl::Uniform<int>(prog, uniform_name + ".cascades_count") = 0;
      } else {
        gl::Uniform<int>(prog, uniform_name + ".cascades_count") = shadow_caster->cascades_count();
        GLuint64 bindless_handle = shadow_caster->shadow_texture().bindless_handle();
        static_assert(sizeof(GLuint64) == sizeof(glm::uvec2), "Expected 32 bit ints");
        gl::Uniform<glm::uvec2>(prog, uniform_name + ".shadowMapId") = *reinterpret_cast<glm::uvec2*>(&bindless_handle);

        for (int i = 0; i < shadow_caster->cascades_count(); ++i) {
          std::string shadow_cp_uniform_name = uniform_name + ".shadowCP[" + std::to_string(i) + "]";
          gl::Uniform<glm::mat4>(prog, shadow_cp_uniform_name) = shadow_caster->GetProjectionMatrix(i) * shadow_caster->GetCameraMatrix(i);
        }
      }
    }

    current_light_index = 0;
    for (PointLightSource* light : point_light_sources_) {
      if (current_light_index >= kMaxLightCount) {
        break;
      }

      std::string uniform_name = "uPointLights[" + std::to_string(current_light_index++) + "]";
      gl::Uniform<glm::vec3>(prog, uniform_name + ".position") = light->transform().GetPos();
      gl::Uniform<glm::vec3>(prog, uniform_name + ".color") = light->color();
    }

    gl::Uniform<int>(prog, "uDirectionalLightCount") = std::min(directional_light_sources_.size(), kMaxLightCount);
    gl::Uniform<int>(prog, "uPointLightCount") = std::min(point_light_sources_.size(), kMaxLightCount);
    gl::Uniform<glm::vec3>(prog, "w_uCamPos") = glm::vec3{scene()->camera()->transform().GetPos()};
  });
}

Scene::~Scene() {
  // close the physics thread
  physics_thread_should_quit_ = true;
  physics_can_run_.Set();
  physics_thread_.join();

  RemovedFromSceneAll();
}

GLFWwindow* Scene::window() const {
  return engine_->GetWindow();
}

ShaderManager* Scene::shader_manager() const {
  return engine_->GetShaderManager();
}

void Scene::Turn() {
  physics_finished_.WaitOne();
  UpdatePhysicsAll();
  physics_can_run_.Set();

  UpdateAll();
  RenderAll();
  Render2DAll();
}

void Scene::RegisterLightSource(PointLightSource* light) {
  point_light_sources_.insert(light);
}

void Scene::UnregisterLightSource(PointLightSource* light) {
  point_light_sources_.erase(light);
}

void Scene::RegisterLightSource(DirectionalLightSource* light) {
  directional_light_sources_.insert(light);
}

void Scene::UnregisterLightSource(DirectionalLightSource* light) {
  directional_light_sources_.erase(light);
}

size_t Scene::triangle_count() {
  size_t sum_triangle_count = 0;
  for (auto& pair : mesh_cache_) {
    sum_triangle_count += pair.second->GetTriangleCount();
  }
  return sum_triangle_count;
}

void Scene::UpdateAll() {
  game_time_.Tick();
  environment_time_.Tick();
  camera_time_.Tick();

  GameObject::UpdateAll();
}

void Scene::RenderAll() {
  if (camera_) {
    for (DirectionalLightSource* light_source : directional_light_sources_) {
      ShadowCaster* shadow_caster = light_source->shadow_caster();
      if (shadow_caster != nullptr) {
        shadow_caster->FillShadowMap(this);
      }
    }

    gl::DepthFunc(gl::kLess);
    gl::DrawBuffer(gl::kNone);  // don't write into the color buffer
    GameObject::ShadowRenderAll(*camera_);
    gl::DepthFunc(gl::kLequal);
    gl::DrawBuffer(gl::kBack);
    GameObject::RenderAll();
  }
}

void Scene::Render2DAll() {
  gl::TemporarySet capabilities{{{gl::kBlend, true},
                                 {gl::kCullFace, false},
                                 {gl::kDepthTest, false}}};
  gl::BlendFunc(gl::kSrcAlpha, gl::kOneMinusSrcAlpha);

  GameObject::Render2DAll();
}

void Scene::UpdatePhysicsInBackgroundThread() {
  if (bt_world_) {
    bt_world_->stepSimulation(game_time().GetDeltaTime(), 16, btScalar(1.0)/btScalar(60.0));
  }
}

}  // namespace Silice3D
