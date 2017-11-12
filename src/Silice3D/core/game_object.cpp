// Copyright (c) Tamas Csala

#include <Silice3D/core/scene.hpp>
#include <Silice3D/core/game_object.hpp>
#include <Silice3D/core/game_engine.hpp>

namespace Silice3D {

GameObject* GameObject::AddComponent(std::unique_ptr<GameObject>&& component) {
  try {
    GameObject *obj = component.get();
    components_just_added_.push_back(std::move(component));
    obj->parent_ = this;
    obj->transform_->SetParent(transform_.get());
    obj->scene_ = scene_;
    obj->AddedToScene();

    return obj;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return nullptr;
  }
}

void GameObject::EnumerateChildren(bool recursive, const std::function<void(GameObject*)>& processor) {
  for (auto& child : components_) {
    if (child) {
      processor(child.get());
      if (recursive) {
        child->EnumerateChildren(recursive, processor);
      }
    }
  }
}

void GameObject::EnumerateConstChildren(bool recursive, const std::function<void(const GameObject*)>& processor) const {
  for (auto& child : components_) {
    if (child) {
      processor(child.get());
      if (recursive) {
        child->EnumerateConstChildren(recursive, processor);
      }
    }
  }
}

size_t GameObject::GetChildrenCount(bool recursive) const {
  size_t count = 0;
  EnumerateConstChildren (recursive, [&count] (const GameObject*) {
    count++;
  });
  return count;
}

void GameObject::RenderRecursive() {
  if (!enabled_) { return; }

  Render();
  for (auto& component : components_) {
    component->RenderRecursive();
  }
}

void GameObject::RenderDepthOnlyRecursive(const ICamera& camera) {
  if (!enabled_) { return; }

  RenderDepthOnly(camera);
  for (auto& component : components_) {
    component->RenderDepthOnlyRecursive(camera);
  }
}

void GameObject::Render2DRecursive() {
  if (!enabled_) { return; }

  Render2D();
  for (auto& component : components_) {
    component->Render2DRecursive();
  }
}

void GameObject::ScreenResizedRecursive(size_t width, size_t height) {
  if (!enabled_) { return; }

  ScreenResized(width, height);
  for (auto& component : components_) {
    component->ScreenResizedRecursive(width, height);
  }
}

void GameObject::UpdateRecursive() {
  if (!enabled_) { return; }

  InternalUpdate();
  Update();
  for (auto& component : components_) {
    component->UpdateRecursive();
  }
}

void GameObject::UpdatePhysicsRecursive() {
  if (!enabled_) { return; }

  UpdatePhysics();
  for (auto& component : components_) {
    component->UpdatePhysicsRecursive();
  }
}

void GameObject::AddedToSceneRecursive() {
  if (!enabled_) { return; }

  AddedToScene();
  for (auto& component : components_) {
    component->AddedToSceneRecursive();
  }
}

void GameObject::RemovedFromSceneRecursive() {
  if (!enabled_) { return; }

  RemovedFromScene();
  for (auto& component : components_) {
    component->RemovedFromSceneRecursive();
  }
}

void GameObject::KeyActionRecursive(int key, int scancode, int action, int mods) {
  if (!enabled_) { return; }

  KeyAction(key, scancode, action, mods);
  for (auto& component : components_) {
    component->KeyActionRecursive(key, scancode, action, mods);
  }
}

void GameObject::CharTypedRecursive(unsigned codepoint) {
  if (!enabled_) { return; }

  CharTyped(codepoint);
  for (auto& component : components_) {
    component->CharTypedRecursive(codepoint);
  }
}

void GameObject::MouseScrolledRecursive(double xoffset, double yoffset) {
  if (!enabled_) { return; }

  MouseScrolled(xoffset, yoffset);
  for (auto& component : components_) {
    component->MouseScrolledRecursive(xoffset, yoffset);
  }
}

void GameObject::MouseButtonPressedRecursive(int button, int action, int mods) {
  if (!enabled_) { return; }

  MouseButtonPressed(button, action, mods);
  for (auto& component : components_) {
    component->MouseButtonPressedRecursive(button, action, mods);
  }
}

void GameObject::MouseMovedRecursive(double xpos, double ypos) {
  if (!enabled_) { return; }

  MouseMoved(xpos, ypos);
  for (auto& component : components_) {
    component->MouseMovedRecursive(xpos, ypos);
  }
}

void GameObject::InternalUpdate() {
  RemoveComponents();
  AddNewComponents();
}

void GameObject::AddNewComponents() {
  if (!components_just_added_.empty()) {
    // make sure all the componenets just enabled are aware of the screen's size
    int width, height;
    glfwGetWindowSize(GetScene()->GetWindow(), &width, &height);
    for (const auto& component : components_just_added_) {
      component->ScreenResizedRecursive(width, height);
    }

    // move them to their new place
    for (auto& component : components_just_added_) {
      components_.push_back(std::move(component));
    }

    components_just_added_.clear();
  }
}

void GameObject::RemoveComponents() {
  if (!components_to_remove_.empty()) {
    for (GameObject* go : components_to_remove_) {
      go->RemovedFromSceneRecursive();
    }

    components_.erase(std::remove_if(components_.begin(), components_.end(),
      [&](const std::unique_ptr<GameObject>& go_ptr){
        return components_to_remove_.find(go_ptr.get()) != components_to_remove_.end();
      }), components_.end());

    components_to_remove_.clear();
  }
}

bool GameObject::StealComponent(GameObject* go) {
  if (!go) { return false; }
  GameObject* parent = go->GetParent();
  if (!parent) { return false; }

  for (auto& component : parent->components_) {
    if (component.get() == go) {
      components_just_added_.push_back(std::move(component));
      // The move leaves a nullptr in the parent->components_
      // that should be removed, as it decrases performance
      parent->RemoveComponent(nullptr);
      component->parent_ = this;
      component->transform_->SetParent(transform_.get());
      component->scene_ = scene_;
      if (go->GetScene() != GetScene()) {
        component->AddedToScene();
      }
      return true;
    }
  }
  return false;
}

void GameObject::RemoveComponent(GameObject* component_to_remove) {
  if (component_to_remove) {
    components_to_remove_.insert(component_to_remove);
  }
}

}  // namespace Silice3D
