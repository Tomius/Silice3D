// Copyright (c) Tamas Csala

#ifndef SILICE3D_MESH_ASSIMP_HPP_
#define SILICE3D_MESH_ASSIMP_HPP_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Silice3D/common/glm.hpp>

// Conversion between assimp and glm matrices

namespace Silice3D {

/// Converts an assimp aiMatrix4x4 to glm mat4
/// @param m - the matrix to convert
static inline glm::mat4 convertMatrix(const aiMatrix4x4& m) {
  return glm::transpose(glm::make_mat4(&m.a1));
}

/// Converts an assimp aiMatrix3x3 to glm mat4
/// @param m - the matrix to convert
static inline glm::mat4 convertMatrix(const aiMatrix3x3& m) {
  return glm::mat4(glm::transpose(glm::make_mat3(&m.a1)));
}

/// Converts a glm mat4 to an assimp aiMatrix4x4
/// @param m - the matrix to convert
static inline aiMatrix4x4 convertMatrix(const glm::mat4& m) {
  return aiMatrix4x4 {
    m[0][0], m[1][0], m[2][0], m[3][0],
    m[0][1], m[1][1], m[2][1], m[3][1],
    m[0][2], m[1][2], m[2][2], m[3][2],
    m[0][3], m[1][3], m[2][3], m[3][3]
  };
}

}

#endif  // SILICE3D_ASSIMP_H_
