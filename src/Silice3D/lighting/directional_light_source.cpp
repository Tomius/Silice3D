// Copyright (c) Tamas Csala

#include <Silice3D/lighting/directional_light_source.hpp>
#include <Silice3D/lighting/shadow_caster.hpp>
#include <Silice3D/core/scene.hpp>

namespace Silice3D {

DirectionalLightSource::DirectionalLightSource(GameObject* parent, const glm::vec3& color)
  : LightSource(parent, color)
  , shadow_caster_{nullptr}
{ }

DirectionalLightSource::DirectionalLightSource(GameObject* parent, const glm::vec3& color,
                                               size_t shadow_map_size, size_t shadow_cascades_count)
  : LightSource(parent, color)
  , shadow_caster_{AddComponent<ShadowCaster>(shadow_map_size, shadow_cascades_count)}
{ }

void DirectionalLightSource::set_no_shadow_caster() {
  if (shadow_caster_ != nullptr) {
    RemoveComponent(shadow_caster_);
  }

  shadow_caster_ = nullptr;
}

void DirectionalLightSource::set_shadow_caster(size_t shadow_map_size, size_t shadow_cascades_count) {
  if (shadow_caster_ != nullptr) {
    RemoveComponent(shadow_caster_);
  }

  shadow_caster_ = AddComponent<ShadowCaster>(shadow_map_size, shadow_cascades_count);
}

void DirectionalLightSource::AddedToScene() {
  scene()->RegisterLightSource(this);
}

void DirectionalLightSource::RemovedFromScene() {
  scene()->UnregisterLightSource(this);
}


} // namespace Silice3D
