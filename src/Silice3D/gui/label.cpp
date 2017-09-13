// Copyright (c), Tamas Csala

#include <glad/glad.h>
#include <oglwrap/oglwrap.h>

#ifdef OGLWRAP_DEBUG
  #define GLT_DEBUG
  #define GLT_DEBUG_PRINT
#endif
#define GLT_MANUAL_VIEWPORT
#include <glText/gltext.h>  // This must be included only from 1 cpp file!

#include <Silice3D/gui/label.hpp>

namespace Silice3D {

Label::Label(GameObject* parent, const std::string& text,
      const glm::vec2& pos, const glm::vec4& color)
    : GameObject(parent)
    , pos_(pos)
    , scale_(1.0)
    , color_(color)
    , text_(gltCreateText()) {
  set_text(text);
}

Label::~Label() {
  gltDeleteText(text_);
}

glm::vec2 Label::position() const {
  return pos_;
}

void Label::set_position(glm::vec2 pos) {
  pos_ = pos;
}

std::string Label::text() const {
  return gltGetText(text_);
}

void Label::set_text(const std::string& text) {
  gltSetText(text_, text.c_str());
}

glm::vec4 Label::color() const {
  return color_;
}

void Label::set_color(const glm::vec4& color) {
  color_ = color;
}

float Label::scale() const {
  return scale_;
}

void Label::set_scale(float scale) {
  scale_ = scale;
}

HorizontalAlignment Label::horizontal_alignment() const {
  return horizontal_alignment_;
}

void Label::set_horizontal_alignment(const HorizontalAlignment& align) {
  horizontal_alignment_ = align;
}

VerticalAlignment Label::vertical_alignment() const {
  return vertical_alignment_;
}

void Label::set_vertical_alignment(const VerticalAlignment& align) {
  vertical_alignment_ = align;
}

bool Label::InitializeTextRendering() {
  return gltInit();
}

void Label::TerminateTextRendering() {
  gltTerminate();
}

void Label::ScreenResized(size_t width, size_t height) {
  gltViewport(width, height);
}

void Label::Render2D() {
  int horizontal_alignment = 0;
  switch (horizontal_alignment_) {
    case HorizontalAlignment::kLeft: horizontal_alignment = GLT_LEFT; break;
    case HorizontalAlignment::kCenter: horizontal_alignment = GLT_CENTER; break;
    case HorizontalAlignment::kRight: horizontal_alignment = GLT_RIGHT; break;
  }
  int vertical_alignment = 0;
  switch (vertical_alignment_) {
    case VerticalAlignment::kBottom: vertical_alignment = GLT_BOTTOM; break;
    case VerticalAlignment::kCenter: vertical_alignment = GLT_CENTER; break;
    case VerticalAlignment::kTop: vertical_alignment = GLT_TOP; break;
  }

  gltColor(color_.x, color_.g, color_.b, color_.a);
  gltDrawText2DAligned (text_, pos_.x, pos_.y, scale_,
                        horizontal_alignment,
                        vertical_alignment);
  OGLWRAP_CHECK_ERROR ();
}

} // namespace Silice3D
