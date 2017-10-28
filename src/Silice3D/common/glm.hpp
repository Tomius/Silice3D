// Copyright (c) Tamas Csala

#ifndef COMMON_GLM_HPP_
#define COMMON_GLM_HPP_

#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

template <typename T>
/// Prints a glm vector to a given ostream.
/** @param os - The ostream.
  * @param v - The vector. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tvec2<T>& v) {
  os << v.x << ", " << v.y;
  return os;
}

template <typename T>
/// Prints a glm vector to a given ostream.
/** @param os - The ostream.
  * @param v - The vector. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tvec3<T>& v) {
  os << v.x << ", " << v.y << ", " << v.z;
  return os;
}

template <typename T>
/// Prints a glm vector to a given ostream.
/** @param os - The ostream.
  * @param v - The vector. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tvec4<T>& v) {
  os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
  return os;
}

template <typename T>
/// Prints a glm vector to a given ostream.
/** @param os - The ostream.
  * @param v - The vector. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tquat<T>& v) {
  os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
  return os;
}

template <typename T>
/// Prints a glm matrix to a given ostream.
/** @param os - The ostream.
  * @param mat - The matrix. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tmat2x2<T>& mat) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      os << mat[j][i];
      if (i != 1 || j != 1) {
        os  << ", ";
      }
    }
    os << std::endl;
  }
  return os;
}

template <typename T>
/// Prints a glm matrix to a given ostream.
/** @param os - The ostream.
  * @param mat - The matrix. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tmat3x3<T>& mat) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      os << mat[j][i];
      if (i != 2 || j != 2) {
        os  << ", ";
      }
    }
    os << std::endl;
  }
  return os;
}

template <typename T>
/// Prints a glm matrix to a given ostream.
/** @param os - The ostream.
  * @param mat - The matrix. */
static inline std::ostream& operator<<(std::ostream& os, const glm::tmat4x4<T>& mat) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      os << mat[j][i];
      if (i != 3 || j != 3) {
        os  << ", ";
      }
    }
    os << std::endl;
  }
  return os;
}

#endif
