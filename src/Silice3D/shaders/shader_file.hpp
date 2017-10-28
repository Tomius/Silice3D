// Copyright (c) Tamas Csala

#ifndef SILICE3D_SHADERS_SHADER_FILE_HPP_
#define SILICE3D_SHADERS_SHADER_FILE_HPP_

#define OGLWRAP_DEFAULT_SHADER_PATH "src/glsl/"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <Silice3D/common/glm.hpp>
#include <oglwrap/oglwrap.h>

namespace Silice3D {

class ShaderProgram;
class ShaderManager;

class ShaderFile : public gl::Shader {
 public:
  ShaderFile(ShaderManager& shader_manager,
             std::string filename,
             const ShaderFile* included_from = nullptr)
      : ShaderFile(shader_manager, filename,
                   gl::ShaderSource{filename}, included_from)
  { }

  ShaderFile(ShaderManager& shader_manager,
             std::string filename,
             const gl::ShaderSource& src,
             const ShaderFile* included_from = nullptr);

  void SetUpdateFunc(std::function<void(const gl::Program&)> func);

  void Update(const gl::Program& prog) const;

 private:
  std::function<void(const gl::Program&)> update_func_;
  std::vector<ShaderFile*> includes_;
  std::string exports_;

  void FindExports(std::string &src);
  void FindIncludes(std::string &src, ShaderManager& shader_manager);

  static gl::ShaderType GetShaderType(const std::string& filename,
                                      const ShaderFile* included_from,
                                      std::string* filename_with_correct_extension = nullptr);

  friend class ShaderProgram;
  friend class ShaderManager;
};

}  // namespace Silice3D

#endif  // SILICE3D_SHADERS_SHADER_FILE_HPP_
