// Copyright (c) Tamas Csala

#include <Silice3D/shaders/shader_program.hpp>

namespace Silice3D {

void ShaderProgram::Update() const {
  for (auto shader : shaders_) {
    shader->Update(*this);
  }
}

// Depth First Search for all the included files, recursively
ShaderProgram& ShaderProgram::AttachShader(ShaderFile *shader) {
  if (shaders_.insert(shader).second) {
    for (auto include : shader->includes_) {
      AttachShader(include);
    }
  }
  return *this;
}

const gl::Program& ShaderProgram::link() {
  for (auto shader_file : shaders_) {
    const gl::Shader& shader = *shader_file;
    gl::Program::attachShader(shader);
  }
  gl::Program::link();

  return *this;
}

}  // namespace Silice3D
