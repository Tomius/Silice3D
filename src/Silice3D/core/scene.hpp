// Copyright (c) Tamas Csala

#ifndef SILICE3D_CORE_SCENE_HPP_
#define SILICE3D_CORE_SCENE_HPP_

#include <map>
#include <vector>
#include <memory>
#include <thread>
#include <btBulletDynamicsCommon.h>

#include <Silice3D/common/timer.hpp>
#include <Silice3D/common/auto_reset_event.hpp>
#include <Silice3D/camera/icamera.hpp>
#include <Silice3D/core/game_object.hpp>
#include <Silice3D/lighting/point_light_source.hpp>
#include <Silice3D/lighting/directional_light_source.hpp>
#include <Silice3D/mesh/imesh_object_renderer.hpp>

namespace Silice3D {

class Scene;
class GameEngine;
class ShaderManager;

class Scene : public GameObject {
 public:
  Scene(GameEngine* engine);
  ~Scene();

  virtual float GetGravity() const { return 9.81f; }

  const Timer& GetGameTime() const { return game_time_; }
  Timer& GetGameTime() { return game_time_; }

  const Timer& GetEnvironmentTime() const { return environment_time_; }
  Timer& GetEnvironmentTime() { return environment_time_; }

  const Timer& GetCameraTime() const { return camera_time_; }
  Timer& GetCameraTime() { return camera_time_; }

  const ICamera* GetCamera() const { return camera_; }
  ICamera* GetCamera() { return camera_; }
  void SetCamera(ICamera* camera) { camera_ = camera; }

  GLFWwindow* GetWindow() const;

  ShaderManager* GetShaderManager() const;

  GameEngine* GetEngine() const { return engine_; }

  const btDynamicsWorld* GetBtWorld() const { return bt_world_.get(); }
  btDynamicsWorld* GetBtWorld() { return bt_world_.get(); }

  MeshRendererCache* GetMeshCache() { return &mesh_cache_; }

  size_t GetTriangleCount();

  virtual void Turn();

  void RegisterLightSource(PointLightSource* light);
  void UnregisterLightSource(PointLightSource* light);

  void RegisterLightSource(DirectionalLightSource* light);
  void UnregisterLightSource(DirectionalLightSource* light);

 protected:
  ICamera* camera_;
  Timer game_time_, environment_time_, camera_time_;
  GameEngine* engine_;

  // Mesh loading
  MeshRendererCache mesh_cache_;

  // Lighting
  std::set<PointLightSource*> point_light_sources_;
  std::set<DirectionalLightSource*> directional_light_sources_;

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

  virtual void UpdateRecursive() override;
  virtual void RenderRecursive() override;
  virtual void Render2DRecursive() override;

  virtual void UpdatePhysicsInBackgroundThread();
};

}  // namespace Silice3D


#endif
