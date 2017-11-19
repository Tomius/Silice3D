// Copyright (c) Tamas Csala

#include <Silice3D/shaders/shader_manager.hpp>
#include <Silice3D/shaders/builtin/bicubic_sampling.glsl>
#include <Silice3D/shaders/builtin/debug_shape.frag>
#include <Silice3D/shaders/builtin/debug_shape.vert>
#include <Silice3D/shaders/builtin/debug_texture.frag>
#include <Silice3D/shaders/builtin/debug_texture.vert>
#include <Silice3D/shaders/builtin/lighting.frag>
#include <Silice3D/shaders/builtin/mesh.frag>
#include <Silice3D/shaders/builtin/mesh.vert>
#include <Silice3D/shaders/builtin/mesh_shadow.frag>
#include <Silice3D/shaders/builtin/post_process.frag>
#include <Silice3D/shaders/builtin/shadow.frag>
#include <Silice3D/shaders/builtin/shadow.vert>


namespace Silice3D {

ShaderManager::ShaderManager() {
  // Publish builtin shaders
  gl::ShaderSource bicubic_sampling_glsl_shader_source;
  bicubic_sampling_glsl_shader_source.set_source_file("Silice3D/bicubic_sampling.glsl");
  bicubic_sampling_glsl_shader_source.set_source(bicubic_sampling_glsl_shader_string);
  PublishShader("Silice3D/bicubic_sampling.frag", bicubic_sampling_glsl_shader_source);
  PublishShader("Silice3D/bicubic_sampling.vert", bicubic_sampling_glsl_shader_source);

  gl::ShaderSource debug_shape_frag_shader_source;
  debug_shape_frag_shader_source.set_source_file("Silice3D/debug_shape.frag");
  debug_shape_frag_shader_source.set_source(debug_shape_frag_shader_string);
  PublishShader(debug_shape_frag_shader_source.source_file(), debug_shape_frag_shader_source);

  gl::ShaderSource debug_shape_vert_shader_source;
  debug_shape_vert_shader_source.set_source_file("Silice3D/debug_shape.vert");
  debug_shape_vert_shader_source.set_source(debug_shape_vert_shader_string);
  PublishShader(debug_shape_vert_shader_source.source_file(), debug_shape_vert_shader_source);

  gl::ShaderSource debug_texture_frag_shader_source;
  debug_texture_frag_shader_source.set_source_file("Silice3D/debug_texture.frag");
  debug_texture_frag_shader_source.set_source(debug_texture_frag_shader_string);
  PublishShader(debug_texture_frag_shader_source.source_file(), debug_texture_frag_shader_source);

  gl::ShaderSource debug_texture_vert_shader_source;
  debug_texture_vert_shader_source.set_source_file("Silice3D/debug_texture.vert");
  debug_texture_vert_shader_source.set_source(debug_texture_vert_shader_string);
  PublishShader(debug_texture_vert_shader_source.source_file(), debug_texture_vert_shader_source);

  gl::ShaderSource post_process_frag_shader_source;
  post_process_frag_shader_source.set_source_file("Silice3D/post_process.frag");
  post_process_frag_shader_source.set_source(post_process_frag_shader_string);
  PublishShader(post_process_frag_shader_source.source_file(), post_process_frag_shader_source);

  gl::ShaderSource lighting_frag_shader_source;
  lighting_frag_shader_source.set_source_file("Silice3D/lighting.frag");
  lighting_frag_shader_source.set_source(lighting_frag_shader_string);
  PublishShader(lighting_frag_shader_source.source_file(), lighting_frag_shader_source);

  gl::ShaderSource mesh_frag_shader_source;
  mesh_frag_shader_source.set_source_file("Silice3D/mesh.frag");
  mesh_frag_shader_source.set_source(mesh_frag_shader_string);
  PublishShader(mesh_frag_shader_source.source_file(), mesh_frag_shader_source);

  gl::ShaderSource mesh_vert_shader_source;
  mesh_vert_shader_source.set_source_file("Silice3D/mesh.vert");
  mesh_vert_shader_source.set_source(mesh_vert_shader_string);
  PublishShader(mesh_vert_shader_source.source_file(), mesh_vert_shader_source);

  gl::ShaderSource mesh_shadow_frag_shader_source;
  mesh_shadow_frag_shader_source.set_source_file("Silice3D/mesh_shadow.frag");
  mesh_shadow_frag_shader_source.set_source(mesh_shadow_frag_shader_string);
  PublishShader(mesh_shadow_frag_shader_source.source_file(), mesh_shadow_frag_shader_source);

  gl::ShaderSource shadow_frag_shader_source;
  shadow_frag_shader_source.set_source_file("Silice3D/shadow.frag");
  shadow_frag_shader_source.set_source(shadow_frag_shader_string);
  PublishShader(shadow_frag_shader_source.source_file(), shadow_frag_shader_source);

  gl::ShaderSource shadow_vert_shader_source;
  shadow_vert_shader_source.set_source_file("Silice3D/shadow.vert");
  shadow_vert_shader_source.set_source(shadow_vert_shader_string);
  PublishShader(shadow_vert_shader_source.source_file(), shadow_vert_shader_source);
}

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
  shaders_[shader->source_file_name()] = std::unique_ptr<ShaderFile>{shader};
  return shader;
}

}  // namespace Silice3D
