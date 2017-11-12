// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADERS_SHADER_MANAGER_HPP_
#define SILICE3D_SHADERS_SHADER_MANAGER_HPP_

#include <Silice3D/shaders/shader_file.hpp>
#include <Silice3D/shaders/shader_program.hpp>

namespace Silice3D {

class ShaderManager {
 public:
  // Publishes a shader to be accessed in the future, by GetShader
  ShaderFile* PublishShader(const std::string& shader_name,
                            const gl::ShaderSource& src);

  // Retrieves the shader that was published with the given name,
  // or if there was no such shader, tries to load it from file.
  ShaderFile* GetShader(const std::string& shader_name,
                        const ShaderFile* included_from = nullptr);
 private:
  std::map<std::string, std::unique_ptr<ShaderFile>> shaders_;

  template<typename... Args>
  ShaderFile* LoadShader(Args&&... args);
};

}  // namespace Silice3D

#endif  // SILICE3D_SHADERS_SHADER_MANAGER_HPP_
