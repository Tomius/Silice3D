// Copyright (c) Tamas Csala

#include <Silice3D/shaders/shader_manager.hpp>

namespace Silice3D {

ShaderFile* ShaderManager::PublishShader(const std::string& filename,
                                         const gl::ShaderSource& src) {
  return LoadShader(*this, filename, src);
}

ShaderFile* ShaderManager::GetShader(const std::string& filename,
                                     const ShaderFile* included_from) {
  std::string filename_with_correct_extension;
  ShaderFile::GetShaderType(filename, included_from, &filename_with_correct_extension);

  auto iter = shaders_.find(filename_with_correct_extension);
  if (iter != shaders_.end()) {
    return iter->second.get();
  } else {
    ShaderFile* shader = LoadShader(*this, filename, included_from);
    return shader;
  }
}

template<typename... Args>
ShaderFile* ShaderManager::LoadShader(Args&&... args) {
  auto shader = new ShaderFile{std::forward<Args>(args)...};
  assert(shaders_.find(shader->source_file_name()) == shaders_.end());
  shaders_[shader->source_file_name()] = std::unique_ptr<ShaderFile>{shader};
  return shader;
}

}  // namespace Silice3D
