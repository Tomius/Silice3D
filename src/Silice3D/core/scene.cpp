// Copyright (c) Tamas Csala

#include <Silice3D/common/oglwrap.hpp>
#include <Silice3D/core/scene.hpp>
#include <Silice3D/core/game_engine.hpp>
#include <Silice3D/lighting/shadow_caster.hpp>

namespace Silice3D {

// Because btDiscreteDynamicsWorld has O(n^2) destructor for n objects by default...
class btFastDestructableDynamicsWorld : public btDiscreteDynamicsWorld {
public:
  using btDiscreteDynamicsWorld::btDiscreteDynamicsWorld;

  void aboutToDestruct() {
    m_nonStaticRigidBodies = btAlignedObjectArray<btRigidBody*>();
    m_collisionObjects = btAlignedObjectArray<btCollisionObject*>();
    m_aboutToDestruct = true;
  }

  virtual void removeRigidBody(btRigidBody *body) override {
    if (!m_aboutToDestruct) {
      btDiscreteDynamicsWorld::removeRigidBody(body);
    }
  }

  virtual void removeCollisionObject(btCollisionObject *collisionObject) override {
    if (!m_aboutToDestruct) {
      btDiscreteDynamicsWorld::removeCollisionObject(collisionObject);
    } else {
      btBroadphaseProxy* bp = collisionObject->getBroadphaseHandle();
      if (bp) {
        btAlignedFree(bp);
        collisionObject->setBroadphaseHandle(0);
      }
    }
  }

private:
  bool m_aboutToDestruct = false;
};

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
  SetScene(this);

  { // Bullet initilization
    bt_collision_config_ = make_unique<btDefaultCollisionConfiguration>();
    bt_dispatcher_ = make_unique<btCollisionDispatcher>(bt_collision_config_.get());
    bt_broadphase_ = make_unique<btDbvtBroadphase>();
    bt_solver_ = make_unique<btSequentialImpulseConstraintSolver>();

    bt_world_ = make_unique<btFastDestructableDynamicsWorld>(
        bt_dispatcher_.get(), bt_broadphase_.get(),
        bt_solver_.get(), bt_collision_config_.get());
    bt_world_->setGravity(btVector3(0, -9.81, 0));
  }

  GetShaderManager()->GetShader("Silice3D/lighting.frag")->SetUpdateFunc([this](const gl::Program& prog) {
    static constexpr const size_t kMaxDirLightCount = 16;
    static constexpr const size_t kMaxPointLightCount = 128;

    size_t current_light_index = 0;
    for (DirectionalLightSource* light : directional_light_sources_) {
      if (current_light_index >= kMaxDirLightCount) {
        break;
      }

      std::string uniform_name = "uDirectionalLights[" + std::to_string(current_light_index++) + "]";
      gl::Uniform<glm::vec3>(prog, uniform_name + ".direction") = light->GetTransform().GetPos();
      gl::Uniform<glm::vec3>(prog, uniform_name + ".color") = light->GetColor();
      ShadowCaster* shadow_caster = light->GetShadowCaster();
      if (shadow_caster == nullptr) {
        gl::Uniform<int>(prog, uniform_name + ".cascades_count") = 0;
      } else {
        gl::Uniform<int>(prog, uniform_name + ".cascades_count") = shadow_caster->GetCascadesCount();
        GLuint64 bindless_handle = shadow_caster->GetShadowTexture().bindless_handle();
        static_assert(sizeof(GLuint64) == sizeof(glm::uvec2), "Expected 32 bit ints");
        gl::Uniform<glm::uvec2>(prog, uniform_name + ".shadowMapId") = *reinterpret_cast<glm::uvec2*>(&bindless_handle);

        for (int i = 0; i < shadow_caster->GetCascadesCount(); ++i) {
          std::string shadow_cp_uniform_name = uniform_name + ".shadowCP[" + std::to_string(i) + "]";
          gl::Uniform<glm::mat4>(prog, shadow_cp_uniform_name) = shadow_caster->GetProjectionMatrix(i) * shadow_caster->GetCameraMatrix(i);
        }
      }
    }

    current_light_index = 0;
    for (PointLightSource* light : point_light_sources_) {
      if (current_light_index >= kMaxPointLightCount) {
        break;
      }

      std::string uniform_name = "uPointLights[" + std::to_string(current_light_index++) + "]";
      gl::Uniform<glm::vec3>(prog, uniform_name + ".position") = light->GetTransform().GetPos();
      gl::Uniform<glm::vec3>(prog, uniform_name + ".color") = light->GetColor();
      gl::Uniform<glm::vec3>(prog, uniform_name + ".attenuation") = light->GetAttenuation();
    }

    gl::Uniform<int>(prog, "uDirectionalLightCount") = std::min(directional_light_sources_.size(), kMaxDirLightCount);
    gl::Uniform<int>(prog, "uPointLightCount") = std::min(point_light_sources_.size(), kMaxPointLightCount);
    gl::Uniform<glm::vec3>(prog, "w_uCamPos") = glm::vec3{GetScene()->GetCamera()->GetTransform().GetPos()};
  });
}

Scene::~Scene() {
  // Close the physics thread
  physics_thread_should_quit_ = true;
  physics_can_run_.Set();
  physics_thread_.join();

  // Signal object's that they will be removed from the scene
  dynamic_cast<btFastDestructableDynamicsWorld*>(bt_world_.get())->aboutToDestruct();
  RemovedFromSceneRecursive();
}

GLFWwindow* Scene::GetWindow() const {
  return engine_->GetWindow();
}

ShaderManager* Scene::GetShaderManager() const {
  return engine_->GetShaderManager();
}

void Scene::Turn() {
  physics_finished_.WaitOne();
  UpdatePhysicsRecursive();
  physics_can_run_.Set();

  UpdateRecursive();
  RenderRecursive();
  Render2DRecursive();
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

size_t Scene::GetTriangleCount() {
  size_t sum_triangle_count = 0;
  for (auto& pair : mesh_cache_) {
    sum_triangle_count += pair.second->GetTriangleCount();
  }
  return sum_triangle_count;
}

void Scene::UpdateRecursive() {
  game_time_.Tick();
  environment_time_.Tick();
  camera_time_.Tick();

  GameObject::UpdateRecursive();
}

void Scene::RenderRecursive() {
  if (camera_) {
    for (DirectionalLightSource* light_source : directional_light_sources_) {
      ShadowCaster* shadow_caster = light_source->GetShadowCaster();
      if (shadow_caster != nullptr) {
        shadow_caster->FillShadowMap(this);
      }
    }

    gl::DepthFunc(gl::kLess);
    gl::DrawBuffer(gl::kNone);  // don't write into the color buffer
    GameObject::RenderDepthOnlyRecursive(*camera_);
    // GameObject::RenderRecursive();
    gl::DepthFunc(gl::kLequal);
    gl::DrawBuffer(gl::kBack);
    GameObject::RenderRecursive();
  }
}

void Scene::Render2DRecursive() {
  gl::TemporarySet capabilities{{{gl::kBlend, true},
                                 {gl::kCullFace, false},
                                 {gl::kDepthTest, false}}};
  gl::BlendFunc(gl::kSrcAlpha, gl::kOneMinusSrcAlpha);

  GameObject::Render2DRecursive();
}

void Scene::UpdatePhysicsInBackgroundThread() {
  if (bt_world_) {
    bt_world_->stepSimulation(GetGameTime().GetDeltaTime(), 16, btScalar(1.0)/btScalar(60.0));
  }
}

}  // namespace Silice3D
