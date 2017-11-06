// Copyright (c), Tamas Csala

#include <Silice3D/common/oglwrap.hpp>

#ifdef OGLWRAP_DEBUG
  #define GLT_DEBUG
  #define GLT_DEBUG_PRINT
#endif
#define GLT_MANUAL_VIEWPORT
#include <glText/gltext.h>  // This must be included only from 1 cpp file!

#include <Silice3D/gui/label.hpp>

namespace Silice3D {

Label::Label(GameObject* parent, const std::string& text,
             const glm::vec2& pos, float scale,
             const glm::vec4& color)
    : GameObject(parent)
    , normalized_pos_(pos)
    , display_pos_(0, 0)
    , scale_(scale)
    , color_(color)
    , text_(gltCreateText()) {
  SetText(text);
}

Label::~Label() {
  gltDeleteText(text_);
}

glm::vec2 Label::GetNormalizedPosition() const {
  return normalized_pos_;
}

void Label::SetNormalizedPosition(const glm::vec2& pos) {
  normalized_pos_ = pos;
  display_pos_ = glm::ivec2(GetScene()->GetEngine()->GetWindowSize() * pos);
}

glm::ivec2 Label::GetDisplayPosition() const {
  return display_pos_;
}

void Label::SetDisplayPosition(const glm::ivec2& pos) {
  display_pos_ = pos;
  normalized_pos_ = glm::vec2(GetScene()->GetEngine()->GetWindowSize()) / glm::vec2(pos);
}

std::string Label::GetText() const {
  return gltGetText(text_);
}

void Label::SetText(const std::string& text) {
  gltSetText(text_, text.c_str());
}

glm::vec4 Label::GetColor() const {
  return color_;
}

void Label::SetColor(const glm::vec4& color) {
  color_ = color;
}

float Label::GetScale() const {
  return scale_;
}

void Label::SetScale(float scale) {
  scale_ = scale;
}

HorizontalAlignment Label::GetHorizontalAlignment() const {
  return horizontal_alignment_;
}

void Label::SetHorizontalAlignment(const HorizontalAlignment& align) {
  horizontal_alignment_ = align;
}

VerticalAlignment Label::GetVerticalAlignment() const {
  return vertical_alignment_;
}

void Label::SetVerticalAlignment(const VerticalAlignment& align) {
  vertical_alignment_ = align;
}

bool Label::InitializeTextRendering() {
  return gltInit();
}

void Label::TerminateTextRendering() {
  gltTerminate();
}

void Label::ScreenResizedForTextRendering(size_t width, size_t height) {
  gltViewport(width, height);
}

void Label::ScreenResized(size_t width, size_t height) {
  display_pos_ = glm::ivec2(glm::vec2(width, height) * normalized_pos_);
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
  gltDrawText2DAligned (text_, display_pos_.x, display_pos_.y, scale_,
                        horizontal_alignment,
                        vertical_alignment);
  OGLWRAP_CHECK_ERROR ();
}

} // namespace Silice3D
