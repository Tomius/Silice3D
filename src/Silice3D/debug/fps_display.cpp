// Copyright (c), Tamas Csala

#include <iomanip>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/debug/fps_display.hpp>

namespace Silice3D {

FpsDisplay::FpsDisplay(GameObject* parent)
    : GameObject(parent)
    , accum_time_(scene_->camera_time().dt()) {
  fps_label_ = AddComponent<Label> ("FPS:      ", glm::vec2{1900, 0});
  fps_label_->set_horizontal_alignment(HorizontalAlignment::kRight);
  fps_label_->set_vertical_alignment(VerticalAlignment::kTop);
  fps_label_->set_scale(1.5);

  object_count_label_ = AddComponent<Label> ("Object count:      ", glm::vec2{1900, 40});
  object_count_label_->set_horizontal_alignment(HorizontalAlignment::kRight);
  object_count_label_->set_vertical_alignment(VerticalAlignment::kTop);

  triangle_count_label_ = AddComponent<Label> ("Triangle count:      ", glm::vec2{1900, 55});
  triangle_count_label_->set_horizontal_alignment(HorizontalAlignment::kRight);
  triangle_count_label_->set_vertical_alignment(VerticalAlignment::kTop);

  triangle_per_sec_label_ = AddComponent<Label> ("Triangle per sec:      ", glm::vec2{1900, 70});
  triangle_per_sec_label_->set_horizontal_alignment(HorizontalAlignment::kRight);
  triangle_per_sec_label_->set_vertical_alignment(VerticalAlignment::kTop);
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
      {
        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setw(6) << std::setprecision(2) << fps;
        fps_label_->set_text(ss.str());
      }

      size_t object_count = scene()->children_count(true);
      {
        std::stringstream ss;
        ss << "Object count: " << std::fixed << std::setw(6) << std::setprecision(2) << object_count;
        object_count_label_->set_text(ss.str());
      }

      size_t triangle_count = scene()->triangle_count();
      {
        std::stringstream ss;
        ss << "Triangle count: " << std::fixed << std::setw(6) << std::setprecision(2) << triangle_count / 1024.0 << "K";
        triangle_count_label_->set_text(ss.str());
      }

      {
        std::stringstream ss;
        ss << "Triangle per sec: " << std::fixed << std::setw(6) << std::setprecision(2) << triangle_count*fps / 1024 / 1024 << "M";
        triangle_per_sec_label_->set_text(ss.str());
      }


      sum_frame_num_ += calls_;
      sum_time_ += accum_time_;
    }

    accum_time_ = calls_ = 0;
  }
}

}
