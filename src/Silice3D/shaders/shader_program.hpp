// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADERS_SHADER_PROGRAM_HPP_
#define SILICE3D_SHADERS_SHADER_PROGRAM_HPP_

#include <Silice3D/shaders/shader_file.hpp>

namespace Silice3D {

class ShaderProgram : public gl::Program {
 public:
  //using gl::Program::Program;

  template <typename... Shaders>
  explicit ShaderProgram(ShaderFile *shader, Shaders&&... shaders) {
    AttachShaders(shader, shaders...);
    link();
  }

  void Update() const;

  template<typename... Rest>
  ShaderProgram& AttachShaders(ShaderFile *shader, Rest&&... rest) {
    AttachShader(shader);
    AttachShaders(rest...);
    return *this;
  }
  ShaderProgram& AttachShaders() { return *this; }
  ShaderProgram& AttachShader(ShaderFile *shader);

  virtual const Program& link() override;

 private:
  std::set<ShaderFile*> shaders_;
};

}  // namespace Silice3D

#endif  // SILICE3D_SHADERS_SHADER_PROGRAM_HPP_
