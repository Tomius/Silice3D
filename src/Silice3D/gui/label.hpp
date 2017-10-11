// Copyright (c), Tamas Csala

#ifndef SILICE3D_GUI_LABEL_H_
#define SILICE3D_GUI_LABEL_H_

#include <string>
#include <vector>

#include <Silice3D/core/game_engine.hpp>

class GLTtext;

namespace Silice3D {

enum class HorizontalAlignment {kLeft, kCenter, kRight};
enum class VerticalAlignment {kBottom, kCenter, kTop};

class Label : public GameObject { // TODO: text rendering without a GameObject
 public:
  Label(GameObject* parent, const std::string& text,
        const glm::vec2& pos, float scale = 1.0,
        const glm::vec4& color = glm::vec4{1.0});
  virtual ~Label();

  glm::vec2 normalized_position() const;
  void set_normalized_position(glm::vec2 pos);

  glm::ivec2 display_position() const;
  void set_display_position(glm::ivec2 pos);

  std::string text() const;
  void set_text(const std::string& text);

  glm::vec4 color() const;
  void set_color(const glm::vec4& color);

  float scale() const;
  void set_scale(float scale);

  HorizontalAlignment horizontal_alignment() const;
  void set_horizontal_alignment(const HorizontalAlignment& align);

  VerticalAlignment vertical_alignment() const;
  void set_vertical_alignment(const VerticalAlignment& align);

public:
  static bool InitializeTextRendering();
  static void TerminateTextRendering();
  static void ScreenResizedForTextRendering(size_t width, size_t height);

  virtual void Render2D() override;
  virtual void ScreenResized(size_t width, size_t height) override;

private:
  glm::vec2 normalized_pos_;
  glm::ivec2 display_pos_;
  float scale_;
  glm::vec4 color_;

  HorizontalAlignment horizontal_alignment_ = HorizontalAlignment::kCenter;
  VerticalAlignment vertical_alignment_ = VerticalAlignment::kCenter;

  GLTtext *text_ = nullptr;
};

}  // namespace Silice3D

#endif
