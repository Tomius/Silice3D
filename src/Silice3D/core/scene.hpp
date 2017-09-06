// Copyright (c) Tamas Csala

#ifndef SILICE3D_SCENE_HPP_
#define SILICE3D_SCENE_HPP_

#include <map>
#include <vector>
#include <memory>
#include <thread>
#include <btBulletDynamicsCommon.h>

#include <Silice3D/common/timer.hpp>
#include <Silice3D/common/auto_reset_event.hpp>
#include <Silice3D/camera/icamera.hpp>
#include <Silice3D/core/game_object.hpp>
#include <Silice3D/lighting/light_source.hpp>
#include <Silice3D/mesh/imesh_object_renderer.hpp>

namespace Silice3D {

class Scene;
class GameEngine;
class ShaderManager;

class Scene : public GameObject {
 public:
  Scene(GameEngine* engine, GLFWwindow* window);
  ~Scene();

  virtual float gravity() const { return 9.81f; }

  const Timer& game_time() const { return game_time_; }
  Timer& game_time() { return game_time_; }

  const Timer& environment_time() const { return environment_time_; }
  Timer& environment_time() { return environment_time_; }

  const Timer& camera_time() const { return camera_time_; }
  Timer& camera_time() { return camera_time_; }

  const ICamera* camera() const { return camera_; }
  ICamera* camera() { return camera_; }
  void set_camera(ICamera* camera) { camera_ = camera; }

  const ICamera* shadow_camera() const { return shadow_camera_; }
  ICamera* shadow_camera() { return shadow_camera_; }
  void set_shadow_camera(ICamera* shadow_camera) { shadow_camera_ = shadow_camera; }

  GLFWwindow* window() const { return window_; }
  void set_window(GLFWwindow* window) { window_ = window; }

  ShaderManager* shader_manager() const;

  GameEngine* engine() const { return engine_; }

  const btDynamicsWorld* bt_world() const { return bt_world_.get(); }
  btDynamicsWorld* bt_world() { return bt_world_.get(); }

  MeshRendererCache* mesh_cache() { return &mesh_cache_; }

  unsigned AddLightSource(LightSource light_source);
  const LightSource& GetLightSource(unsigned id) const;
  LightSource& GetLightSource(unsigned id);
  void EnumerateLightSources(std::function<void(const LightSource&)> processor) const;
  bool RemoveLightSource(unsigned id);

  virtual void Turn();

 protected:
  ICamera* camera_;
  ICamera* shadow_camera_;
  Timer game_time_, environment_time_, camera_time_;
  GLFWwindow* window_;
  GameEngine* engine_;
  std::map<unsigned, LightSource> light_sources_;
  MeshRendererCache mesh_cache_;

  // Bullet classes
  std::unique_ptr<btCollisionConfiguration> bt_collision_config_;
  std::unique_ptr<btDispatcher> bt_dispatcher_;
  std::unique_ptr<btBroadphaseInterface> bt_broadphase_;
  std::unique_ptr<btConstraintSolver> bt_solver_;
  std::unique_ptr<btDynamicsWorld> bt_world_;

  // physics thread data
  AutoResetEvent physics_can_run_{false}, physics_finished_{true};
  bool physics_thread_should_quit_;
  std::thread physics_thread_;

  virtual void UpdateAll() override;

  virtual void RenderAll() override;

  virtual void Render2DAll() override;

  virtual void UpdatePhysicsInBackgroundThread();
};

}  // namespace Silice3D


#endif