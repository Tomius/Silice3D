// Copyright (c) Tamas Csala

#include <glad/glad.h>
#include <oglwrap/oglwrap.h>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/core/game_engine.hpp>

namespace Silice3D {

Scene::Scene(GameEngine* engine, GLFWwindow* window)
    : GameObject(nullptr)
    , camera_(nullptr)
    , window_(window)
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
}

Scene::~Scene() {
  // close the physics thread
  physics_thread_should_quit_ = true;
  physics_can_run_.Set();
  physics_thread_.join();

  RemovedFromSceneAll();
}

ShaderManager* Scene::shader_manager() const { return engine_->shader_manager(); }

void Scene::Turn() {
  physics_finished_.WaitOne();
  UpdatePhysicsAll();
  physics_can_run_.Set();

  UpdateAll();
  RenderAll();
  Render2DAll();
}

size_t Scene::triangle_count() {
  size_t sum_triangle_count = 0;
  for (auto& pair : mesh_cache_) {
    sum_triangle_count += pair.second->GetTriangleCount();
  }
  return sum_triangle_count;
}

unsigned Scene::AddLightSource(LightSource light_source) {
  static unsigned id = 1;
  light_sources_[id] = light_source;
  return id++;
}

const LightSource& Scene::GetLightSource(unsigned id) const {
  auto iter = light_sources_.find(id);
  if (iter != light_sources_.end()) {
    return iter->second;
  } else {
    throw std::out_of_range("");
  }
}

LightSource& Scene::GetLightSource(unsigned id) {
  auto iter = light_sources_.find(id);
  if (iter != light_sources_.end()) {
    return iter->second;
  } else {
    throw std::out_of_range("");
  }
}

void Scene::EnumerateLightSources(std::function<void(const LightSource&)> processor) const {
  for (const auto& pair : light_sources_) {
    processor(pair.second);
  }
}

bool Scene::RemoveLightSource(unsigned id) {
  return light_sources_.erase(id) == 1;
}

void Scene::UpdateAll() {
  game_time_.Tick();
  environment_time_.Tick();
  camera_time_.Tick();

  GameObject::UpdateAll();
}

void Scene::RenderAll() {
  if (camera_) { GameObject::RenderAll(); }
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
    bt_world_->stepSimulation(game_time().dt(), 16, btScalar(1.)/btScalar(120.));
  }
}

}  // namespace Silice3D
