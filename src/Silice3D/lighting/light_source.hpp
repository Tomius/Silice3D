// Copyright (c) Tamas Csala

#ifndef SILICE3D_LIGHT_SOURCE_HPP_
#define SILICE3D_LIGHT_SOURCE_HPP_

#include <Silice3D/common/glm.hpp>

struct LightSource {
  // directional lights are directed towards the origin
  enum class Type {kPoint, kDirectional} type = Type::kPoint;
  glm::vec3 position = {0.0, 0.0, 0.0};
  glm::vec3 color = {0.0, 0.0, 0.0};

  LightSource () = default;
  LightSource (Type type, const glm::vec3& pos, const glm::vec3& color)
    : type (type), position (pos), color (color) {}
};

#endif
