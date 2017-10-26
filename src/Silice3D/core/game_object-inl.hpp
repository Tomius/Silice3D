// Copyright (c) Tamas Csala

#ifndef SILICE3D_GAME_OBJECT_INL_HPP_
#define SILICE3D_GAME_OBJECT_INL_HPP_

#include <iostream>

#include <Silice3D/core/game_object.hpp>

namespace Silice3D {

template<typename Transform_t>
GameObject::GameObject(GameObject* parent, const Transform_t& transform)
    : scene_(parent ? parent->scene_ : nullptr), parent_(parent)
    , transform_(new Transform_t{transform})
    , enabled_(true) {
  if (parent && transform.parent() == nullptr) {
    transform_->set_parent(&parent_->transform());
  }
}

template<typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args) {
  static_assert(std::is_base_of<GameObject, T>::value, "Not a GameObject");

  try {
    T *obj = new T(this, std::forward<Args>(args)...);
    AddComponent(std::unique_ptr<GameObject>(obj));
    return obj;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return nullptr;
  }
}

}  // namespace Silice3D

#endif
