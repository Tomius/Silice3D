// Copyright (c) Tamas Csala

#ifndef SILICE3D_DIRECTIONAL_LIGHT_SOURCE_HPP_
#define SILICE3D_DIRECTIONAL_LIGHT_SOURCE_HPP_

#include <Silice3D/lighting/light_source.hpp>

namespace Silice3D {

class ShadowCaster;

// directional lights are directed from trasform().pos() towards the origin
class DirectionalLightSource : public LightSource {
public:
  DirectionalLightSource(GameObject* parent, const glm::vec3& color);
  DirectionalLightSource(GameObject* parent, const glm::vec3& color, size_t shadow_map_size, size_t shadow_cascades_count);

  ShadowCaster* GetShadowCaster() const { return shadow_caster_; }

  void SetNoShadowCaster();
  void SetShadowCaster(size_t shadow_map_size, size_t shadow_cascades_count);

private:
  ShadowCaster* shadow_caster_ = nullptr;

  virtual void AddedToScene() override;
  virtual void RemovedFromScene() override;
};

} // namespace Silice3D

#endif
