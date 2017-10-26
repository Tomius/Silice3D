// Copyright (c) Tamas Csala

#include <Silice3D/lighting/light_source.hpp>

namespace Silice3D {

LightSource::LightSource(GameObject* parent, const glm::vec3& color)
  : GameObject(parent), color_(color)
{ }

} // namespace Silice3D
