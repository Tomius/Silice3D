// Copyright (c) Tamas Csala

#ifndef SILICE3D_COMMON_MATH_HPP_
#define SILICE3D_COMMON_MATH_HPP_

#include <Silice3D/common/glm.hpp>

#ifndef M_PI
  #define M_PI 3.14159265359
#endif
#ifndef M_PI_2
  #define M_PI_2 1.57079632679
#endif

namespace Silice3D {
namespace Math {

static constexpr double kEpsilon = 1e-5;

inline float Rand01() {
  return rand() / float(RAND_MAX);
}

inline glm::vec3 RandomDir() {
  float r = Rand01();
  float latitude = Rand01() * 2*M_PI;
  float longitude = Rand01() * M_PI;
  return glm::vec3{r*sin(longitude)*cos(latitude),
                   r*sin(longitude)*sin(latitude),
                   r*cos(longitude)};
}

template<typename T>
constexpr T Sqr(T v) {
  return v * v;
}

template<typename T>
constexpr T Cube(T v) {
  return v * v * v;
}

} // namespace Math
} // namespace Silice3D

#endif
