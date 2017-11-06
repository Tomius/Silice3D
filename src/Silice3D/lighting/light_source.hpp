// Copyright (c) Tamas Csala

#ifndef SILICE3D_LIGHT_SOURCE_HPP_
#define SILICE3D_LIGHT_SOURCE_HPP_

#include <Silice3D/common/glm.hpp>
#include <Silice3D/core/game_object.hpp>

namespace Silice3D {

class LightSource : public GameObject {
public:
  LightSource (GameObject* parent, const glm::vec3& color);

  glm::vec3 GetColor() const { return color_; }
  void SetColor(const glm::vec3& color) { color_ = color; }

private:
  glm::vec3 color_ = {0.0, 0.0, 0.0};
};

} // namespace Silice3D

#endif
