// Copyright (c) Tamas Csala

#ifndef COLLISION_FRUSTUM_HPP_
#define COLLISION_FRUSTUM_HPP_

#include <Silice3D/collision/plane.hpp>

namespace Silice3D {

struct Frustum {
  Plane planes[6]; // left, right, top, down, near, far
};

} // namespace Silice3D

#endif
