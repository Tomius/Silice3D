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
  // Creates a GameObject with the specified parent and initial transformation
  template<typename Transform_t = Transform>
  explicit GameObject(GameObject* parent,
                      const Transform_t& initial_transform = Transform_t{});

  // Destructs the GameObject
  virtual ~GameObject() = default;

  // Calls the constructor of a T type GameObject with Args... arguments,
  // and sets the created object as a component of this one
  template<typename T, typename... Args>
  T* AddComponent(Args&&... contructor_args);

  // Add a GameObject as a component of this one, and assumes ownership of it.
  virtual GameObject* AddComponent(std::unique_ptr<GameObject>&& component);

  // Detaches a componenent from its parent, and adopts it.
  // Returns true on success.
  virtual bool StealComponent(GameObject* component_to_steal);

  // Detaches a component, that has been previously added to this one
  virtual void RemoveComponent(GameObject* component_to_remove);

  // Calls the processor function with all of the children component of this
  // GameObject. If recursive is true, includes the children's of children too.
  void EnumerateChildren(bool recursive, const std::function<void(GameObject*)>& processor);

  // Calls the processor function with all of the children component of this
  // GameObject. If recursive is true, includes the children's of children too.
  void EnumerateConstChildren(bool recursive, const std::function<void(const GameObject*)>& processor) const;

  // Returns the number of components this GameObjects has. If the recursive
  // paramter is true, returns the size of the hierarciycal tree below this
  // GameObject.
  size_t GetChildrenCount(bool recursive) const;

  // Returns the transformation of this GameObject.
  Transform& GetTransform();
  const Transform& GetTransform() const;

  // Returns the parent GameObject, who owns this one. If this GameObject is a
  // Scene, the parent may be nullptr. To change this ownership, use StealComponent.
  GameObject* GetParent();
  const GameObject* GetParent() const;

  // Returns the Scene this GameObject is attached to.
  Scene* GetScene();
  const Scene* GetScene() const;

  // Callback functions are not called for disabled GameObjects.
  // Below are the setters and getters for this state
  bool IsEnabled() const;
  void SetIsEnabled(bool value);

  // Callback functions for the current GameObject
  virtual void Render() {}
  virtual void RenderDepthOnly(const ICamera& /*camera*/) {}
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

  // Callback functions for the entire tree of componenets owned by this GameObject
  virtual void RenderRecursive();
  virtual void RenderDepthOnlyRecursive(const ICamera& camera);
  virtual void Render2DRecursive();
  virtual void UpdateRecursive();
  virtual void UpdatePhysicsRecursive();
  virtual void AddedToSceneRecursive();
  virtual void RemovedFromSceneRecursive();
  virtual void ScreenResizedRecursive(size_t /*width*/, size_t /*height*/);
  virtual void KeyActionRecursive(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/);
  virtual void CharTypedRecursive(unsigned /*codepoint*/);
  virtual void MouseScrolledRecursive(double /*xoffset*/, double /*yoffset*/);
  virtual void MouseButtonPressedRecursive(int /*button*/, int /*action*/, int /*mods*/);
  virtual void MouseMovedRecursive(double /*xpos*/, double /*ypos*/);

 protected:
  Scene* scene_;
  GameObject* parent_;
  std::unique_ptr<Transform> transform_;
  std::vector<std::unique_ptr<GameObject>> components_;
  std::vector<std::unique_ptr<GameObject>> components_just_added_;
  std::set<GameObject*> components_to_remove_;
  bool enabled_;

  void InternalUpdate();

 private: // Implementation functions
  void AddNewComponents();
  void RemoveComponents();

 private: // Function callable only by Scene
  friend class Scene;
  void SetScene(Scene* scene) { scene_ = scene; }
};

}  // namespace Silice3D

#include <Silice3D/core/game_object-inl.hpp>

#endif
