// Copyright (c) Tamas Csala

#ifndef SILICE3D_CORE_GAME_ENGINE_HPP_
#define SILICE3D_CORE_GAME_ENGINE_HPP_

#include <memory>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/common/make_unique.hpp>
#include <Silice3D/shaders/shader_manager.hpp>

namespace Silice3D {

class GameEngine {
 public:
  enum class WindowMode { kFullScreen, kWindowed };

  GameEngine(const std::string& application_name, WindowMode windowMode);
  ~GameEngine();

  void Run();
  void LoadScene(std::unique_ptr<Scene>&& new_scene);

  Scene* GetScene() { return scene_.get(); }
  GLFWwindow* GetWindow() { return window_; }
  ShaderManager* GetShaderManager() { return shader_manager_.get(); }
  glm::vec2 GetWindowSize();

 private:
  bool minimized_ = false;
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<Scene> new_scene_;
  std::unique_ptr<ShaderManager> shader_manager_;
  GLFWwindow *window_;

  // GLFW Callbacks
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
