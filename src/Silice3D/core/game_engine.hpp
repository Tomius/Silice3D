// Copyright (c) Tamas Csala

#ifndef SILICE3D_GAME_SILICE3D_HPP_
#define SILICE3D_GAME_SILICE3D_HPP_

#include <memory>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/common/make_unique.hpp>
#include <Silice3D/shaders/shader_manager.hpp>

namespace Silice3D {

class GameEngine {
 public:
  GameEngine();
  ~GameEngine();

  void LoadScene(std::unique_ptr<Scene>&& new_scene);

  Scene* scene() { return scene_.get(); }

  GLFWwindow* window() { return window_; }

  ShaderManager* shader_manager() { return shader_manager_.get(); }

  glm::vec2 window_size();

  void Run();

 private:
  bool minimized_ = false;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<Scene> new_scene_;
  std::unique_ptr<ShaderManager> shader_manager_ = make_unique<ShaderManager>();
  GLFWwindow *window_;

  // Callbacks
  static void ErrorCallback(int error, const char* message);

  static void KeyCallback(GLFWwindow* window, int key, int scancode,
                          int action, int mods);

  static void CharCallback(GLFWwindow* window, unsigned codepoint);

  static void ScreenResizeCallback(GLFWwindow* window, int width, int height);

  static void MouseScrolledCallback(GLFWwindow* window, double xoffset,
                                    double yoffset);

  static void MouseButtonPressed(GLFWwindow* window, int button,
                                 int action, int mods);

  static void MouseMoved(GLFWwindow* window, double xpos, double ypos);
};

}  // namespace Silice3D

#endif