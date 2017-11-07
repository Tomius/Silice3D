// Copyright (c) Tamas Csala

#ifndef SILICE3D_CORE_GAME_OBJECT_HPP_
#define SILICE3D_CORE_GAME_OBJECT_HPP_

#include <set>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#include <Silice3D/common/oglwrap.hpp>
#include <Silice3D/common/transform.hpp>

namespace Silice3D {

// TODO: better place for these
constexpr int kShadowTextureSlot = 0;
constexpr int kDiffuseTextureSlot = 1;

class Scene;
class ICamera;

class GameObject {
 public:
  template<typename Transform_t = Transform>
  explicit GameObject(GameObject* parent,
                      const Transform_t& initial_transform = Transform_t{});
  virtual ~GameObject() = default;

  template<typename T, typename... Args>
  T* AddComponent(Args&&... contructor_args);

  virtual GameObject* AddComponent(std::unique_ptr<GameObject>&& component);

  // Detaches a componenent from its parent, and adopts it.
  // Returns true on success.
  virtual bool StealComponent(GameObject* component_to_steal);

  virtual void RemoveComponent(GameObject* component_to_remove);

  void EnumerateChildren(bool recursive, const std::function<void(GameObject*)>& processor);

  void EnumerateConstChildren(bool recursive, const std::function<void(const GameObject*)>& processor) const;

  size_t GetChildrenCount(bool recursive) const;

  Transform& GetTransform() { return *transform_.get(); }
  const Transform& GetTransform() const { return *transform_.get(); }

  GameObject* GetParent() { return parent_; }
  const GameObject* GetParent() const { return parent_; }
  void SetParent(GameObject* parent);

  Scene* GetScene() { return scene_; }
  const Scene* GetScene() const { return scene_; }
  void SetScene(Scene* scene) { scene_ = scene; }

  bool IsEnabled() const { return enabled_; }
  void SetIsEnabled(bool value) { enabled_ = value; }

  virtual void Render() {}
  virtual void ShadowRender(const ICamera& /*shadow_camera*/) {}
  virtual void Render2D() {}
  virtual void Update() {}
  virtual void UpdatePhysics() {}
  virtual void AddedToScene() {}
  virtual void RemovedFromScene() {}
  virtual void ScreenResized(size_t /*width*/, size_t /*height*/) {}
  virtual void KeyAction(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {}
  virtual void CharTyped(unsigned /*codepoint*/) {}
  virtual void MouseScrolled(double /*xoffset*/, double /*yoffset*/) {}
  virtual void MouseButtonPressed(int /*button*/, int /*action*/, int /*mods*/) {}
  virtual void MouseMoved(double /*xpos*/, double /*ypos*/) {}

  virtual void RenderAll();
  virtual void ShadowRenderAll(const ICamera& shadow_camera);
  virtual void Render2DAll();
  virtual void UpdateAll();
  virtual void UpdatePhysicsAll();
  virtual void AddedToSceneAll();
  virtual void RemovedFromSceneAll();
  virtual void ScreenResizedAll(size_t width, size_t height);
  virtual void KeyActionAll(int key, int scancode, int action, int mods);
  virtual void CharTypedAll(unsigned codepoint);
  virtual void MouseScrolledAll(double xoffset, double yoffset);
  virtual void MouseButtonPressedAll(int button, int action, int mods);
  virtual void MouseMovedAll(double xpos, double ypos);

 protected:
  Scene* scene_;
  GameObject* parent_;
  std::unique_ptr<Transform> transform_;
  std::vector<std::unique_ptr<GameObject>> components_;
  std::vector<std::unique_ptr<GameObject>> components_just_added_;
  std::set<GameObject*> components_to_remove_;
  bool enabled_;

  void InternalUpdate();

 private:
  void AddNewComponents();
  void RemoveComponents();
};

}  // namespace Silice3D

#include <Silice3D/core/game_object-inl.hpp>

#endif
