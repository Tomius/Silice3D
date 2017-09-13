// Copyright (c), Tamas Csala

#include <iomanip>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/debug/fps_display.hpp>

namespace Silice3D {

FpsDisplay::FpsDisplay(GameObject* parent)
    : GameObject(parent)
    , accum_time_(scene_->camera_time().dt()) {
  label_ = AddComponent<Label> ("FPS:      ", glm::vec2{1920, 0});
  label_->set_horizontal_alignment(HorizontalAlignment::kRight);
  label_->set_vertical_alignment(VerticalAlignment::kTop);
  label_->set_scale(1.5);
}

FpsDisplay::~FpsDisplay() {
  if (sum_time_ != 0) {
    std::cout << "Average FPS: " << sum_frame_num_ / sum_time_ << std::endl;
  }
}

void FpsDisplay::Update() {
  calls_++;
  accum_time_ += scene_->camera_time().dt();
  if (accum_time_ > kRefreshInterval && accum_time_ != 0) {
    if (first_display_interval_) {
      // The first interval is usually much slower, remove that bias
      first_display_interval_ = false;
    } else {
      double fps = calls_ / accum_time_;
      std::stringstream ss;
      ss << "FPS: " << std::fixed << std::setw(6) << std::setprecision(2) << fps;
      label_->set_text(ss.str());

      sum_frame_num_ += calls_;
      sum_time_ += accum_time_;
    }

    accum_time_ = calls_ = 0;
  }
}

}
