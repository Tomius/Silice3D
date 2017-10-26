// Copyright (c) Tamas Csala

#ifndef SILICE3D_POINT_LIGHT_SOURCE_HPP_
#define SILICE3D_POINT_LIGHT_SOURCE_HPP_

#include <Silice3D/lighting/light_source.hpp>

namespace Silice3D {

// point lights are located at trasform().pos()
class PointLightSource : public LightSource {
public:
  PointLightSource(GameObject* parent, const glm::vec3& color, const glm::vec3& attenuation);

  glm::vec3 attenuation() const { return attenuation_; }
  void set_attenuation(const glm::vec3& attenuation) { attenuation_ = attenuation; }

private:
  // .x: quadratic, .y: linear, .z: constant
  glm::vec3 attenuation_ = {0, 0, 1};

  virtual void AddedToScene() override;
  virtual void RemovedFromScene() override;
};

} // namespace Silice3D

#endif
