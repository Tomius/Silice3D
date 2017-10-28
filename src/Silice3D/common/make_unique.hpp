// Copyright (c) Tamas Csala

#ifndef SILICE3D_COMMON_MAKE_UNIQUE_HPP_
#define SILICE3D_COMMON_MAKE_UNIQUE_HPP_

#include <memory>

namespace Silice3D {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
}

template<typename T>
std::unique_ptr<T> make_unique() {
  return std::unique_ptr<T>{new T{}};
}

}

#endif
