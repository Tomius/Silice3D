// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADERS_SHADER_MANAGER_HPP_
#define SILICE3D_SHADERS_SHADER_MANAGER_HPP_

#include <Silice3D/shaders/shader_file.hpp>
#include <Silice3D/shaders/shader_program.hpp>

namespace Silice3D {

class ShaderManager {
 public:
  ShaderFile* publish(const std::string& filename, const gl::ShaderSource& src);
  ShaderFile* get(const std::string& filename,
                  const ShaderFile* included_from = nullptr);
 private:
  std::map<std::string, std::unique_ptr<ShaderFile>> shaders_;

  template<typename... Args>
  ShaderFile* LoadShader(Args&&... args);
};

}  // namespace Silice3D

#endif  // SILICE3D_SHADERS_SHADER_MANAGER_HPP_
