// Copyright (c) Tamas Csala

#include <Silice3D/lighting/point_light_source.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

PointLightSource::PointLightSource(GameObject* parent,
                                   const glm::vec3& color,
                                   const glm::vec3& attenuation)
  : LightSource(parent, color), attenuation_(attenuation)
{ }

void PointLightSource::AddedToScene() {
  GetScene()->RegisterLightSource(this);
}

void PointLightSource::RemovedFromScene() {
  GetScene()->UnregisterLightSource(this);
}

} // namespace Silice3D
