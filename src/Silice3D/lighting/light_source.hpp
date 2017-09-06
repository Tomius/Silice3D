// Copyright (c) Tamas Csala

#ifndef SILICE3D_LIGHT_SOURCE_HPP_
#define SILICE3D_LIGHT_SOURCE_HPP_

#include <Silice3D/common/glm.hpp>

struct LightSource {
  // directional lights are directed towards the origin
  enum class Type {kPoint, kDirectional} type;
  glm::vec3 position, color;
};

#endif
