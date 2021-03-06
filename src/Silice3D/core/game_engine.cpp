// Copyright (c) Tamas Csala

#include <string>

#include <Silice3D/common/oglwrap.hpp>
#include <GLFW/glfw3.h>

#include <Silice3D/core/game_engine.hpp>
#include <Silice3D/gui/label.hpp>
#include <Silice3D/mesh/mesh_renderer.hpp>

#ifdef USE_DEBUG_CONTEXT
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar *message, const void *userParam) {
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
    std::cout << "OpenGL DebugCallback: " << message << std::endl;
  }
}
#endif

namespace Silice3D {

GameEngine::GameEngine(const std::string& application_name, WindowMode windowMode) {
  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit()) {
    std::terminate();
  }

  // Window creation
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

#ifdef USE_DEBUG_CONTEXT
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  if (windowMode == WindowMode::kFullScreen) {
    window_ = glfwCreateWindow(vidmode->width, vidmode->height,
                               application_name.c_str(), monitor, nullptr);
  } else {
    window_ = glfwCreateWindow(vidmode->width, vidmode->height,
                               application_name.c_str(), nullptr, nullptr);
  }

  if (!window_) {
    std::cerr << "FATAL: Couldn't create a glfw window. Aborting now." << std::endl;
    glfwTerminate();
    std::terminate();
  }

  // Check the created OpenGL context's version
  int ogl_major_version = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MAJOR);
  int ogl_minor_version = glfwGetWindowAttrib(window_, GLFW_CONTEXT_VERSION_MINOR);
  std::cout << "OpenGL version: "  << ogl_major_version << '.' << ogl_minor_version << std::endl;
  int width, height;
  glfwGetFramebufferSize(window_, &width, &height);
  std::cout << "Resolution: "  << width << " x " << height << std::endl;

  if (ogl_major_version < 4 || (ogl_major_version == 4 && ogl_minor_version < 5)) {
    std::cerr << "At least OpenGL version 4.5 is required to run this program\n";
    std::terminate();
  }

  glfwMakeContextCurrent(window_);

  // No V-sync needed.
  glfwSwapInterval(0);

  bool success = gladLoadGL();
  if (!success) {
    std::cerr << "gladLoadGL failed" << std::endl;
    std::terminate();
  }

#ifdef USE_DEBUG_CONTEXT
  glDebugMessageCallback(&DebugCallback, nullptr);
#endif

  success = Label::InitializeTextRendering();
  if (!success) {
    std::cerr << "Label::InitializeTextRendering failed" << std::endl;
    std::terminate();
  }

  MeshRenderer::InitializeMeshDataStorage();

  // Only initialize after the OpenGL context has been created
  shader_manager_ = make_unique<ShaderManager>();

  // OpenGL initialization
  gl::Enable(gl::kDepthTest);
  glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

  // GLFW callbacks
  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, KeyCallback);
  glfwSetCharCallback(window_, CharCallback);
  glfwSetFramebufferSizeCallback(window_, ScreenResizeCallback);
  glfwSetScrollCallback(window_, MouseScrolledCallback);
  glfwSetMouseButtonCallback(window_, MouseButtonPressed);
  glfwSetCursorPosCallback(window_, MouseMoved);
}

GameEngine::~GameEngine() {
  if (window_) {
    shader_manager_.reset();
    scene_.reset();
    new_scene_.reset();
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  MeshRenderer::FreeMeshDataStorage();
  Label::TerminateTextRendering();
  glfwTerminate();
}

void GameEngine::LoadScene(std::unique_ptr<Scene>&& new_scene) {
  new_scene_ = std::move(new_scene);
}

void GameEngine::Run() {
  while (!glfwWindowShouldClose(window_)) {
    if (new_scene_) {
      std::swap(scene_, new_scene_);
      new_scene_ = nullptr;
    }
    if (!minimized_) {
      gl::Clear().Color().Depth();
      scene_->Turn();

      glfwSwapBuffers(window_);
    }

    glfwPollEvents();
  }
}

glm::vec2 GameEngine::GetWindowSize() {
  if (window_) {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    return glm::vec2(width, height);
  } else {
    return glm::vec2{0.0, 0.0};
  }
}

void GameEngine::ErrorCallback(int error, const char* message) {
  std::cerr << message;
}

void GameEngine::KeyCallback(GLFWwindow* window, int key, int scancode,
                             int action, int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
      case GLFW_KEY_F11: {
        bool no_cursor_restriction =
            (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);

        if (no_cursor_restriction) {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
      } break;
      default:
        break;
    }
  }
  GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
  if (game_engine && game_engine->scene_ && !game_engine->new_scene_) {
    game_engine->scene_->KeyActionRecursive(key, scancode, action, mods);
  }
}

void GameEngine::CharCallback(GLFWwindow* window, unsigned codepoint) {
  GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
  if (game_engine && game_engine->scene_ && !game_engine->new_scene_) {
    game_engine->scene_->CharTypedRecursive(codepoint);
  }
}

void GameEngine::ScreenResizeCallback(GLFWwindow* window, int width, int height) {
  gl::Viewport(width, height);
  Silice3D::Label::ScreenResizedForTextRendering(width, height);

  GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
  if (game_engine) {
    if (width == 0 || height == 0) {
      game_engine->minimized_ = true;
    } else {
      game_engine->minimized_ = false;
      if (game_engine->scene_) {
        game_engine->scene_->ScreenResizedRecursive(width, height);
      }
    }
  }
}

void GameEngine::MouseScrolledCallback(GLFWwindow* window,
                                       double xoffset,
                                       double yoffset) {
  GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
  if (game_engine && game_engine->scene_ && !game_engine->new_scene_) {
    game_engine->scene_->MouseScrolledRecursive(xoffset, yoffset);
  }
}

void GameEngine::MouseButtonPressed(GLFWwindow* window, int button,
                                    int action, int mods) {
    GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
    if (game_engine && game_engine->scene_ && !game_engine->new_scene_) {
      game_engine->scene_->MouseButtonPressedRecursive(button, action, mods);
    }
  }

void GameEngine::MouseMoved(GLFWwindow* window, double xpos, double ypos) {
  GameEngine* game_engine = reinterpret_cast<GameEngine*>(glfwGetWindowUserPointer(window));
  if (game_engine && game_engine->scene_ && !game_engine->new_scene_) {
    game_engine->scene_->MouseMovedRecursive(xpos, ypos);
  }
}

}  // namespace Silice3D
