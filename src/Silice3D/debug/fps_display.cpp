// Copyright (c), Tamas Csala

#include <iomanip>

#include <Silice3D/core/scene.hpp>
#include <Silice3D/debug/fps_display.hpp>

namespace Silice3D {

FpsDisplay::FpsDisplay(GameObject* parent)
    : GameObject(parent) {
  fps_label_ = AddComponent<Label> ("FPS:      ", glm::vec2{0.99, 0});
  fps_label_->SetHorizontalAlignment(HorizontalAlignment::kRight);
  fps_label_->SetVerticalAlignment(VerticalAlignment::kTop);
  fps_label_->SetScale(1.5);

  object_count_label_ = AddComponent<Label> ("Object count:      ", glm::vec2{0.99, 0.04});
  object_count_label_->SetHorizontalAlignment(HorizontalAlignment::kRight);
  object_count_label_->SetVerticalAlignment(VerticalAlignment::kTop);

  triangle_count_label_ = AddComponent<Label> ("Triangle count:      ", glm::vec2{0.99, 0.055});
  triangle_count_label_->SetHorizontalAlignment(HorizontalAlignment::kRight);
  triangle_count_label_->SetVerticalAlignment(VerticalAlignment::kTop);

  triangle_per_sec_label_ = AddComponent<Label> ("Triangle per sec:      ", glm::vec2{0.99, 0.07});
  triangle_per_sec_label_->SetHorizontalAlignment(HorizontalAlignment::kRight);
  triangle_per_sec_label_->SetVerticalAlignment(VerticalAlignment::kTop);
}

void FpsDisplay::Update() {
  calls_++;
  accum_time_ += GetScene()->GetCameraTime().GetDeltaTime();
  if (accum_time_ > kRefreshInterval && accum_time_ != 0) {
    if (first_display_interval_) {
      // The first interval is usually much slower, remove that bias
      first_display_interval_ = false;
    } else {
      double fps = calls_ / accum_time_;
      {
        std::stringstream ss;
        ss << "FPS: " << std::fixed << std::setw(6) << std::setprecision(2) << fps;
        fps_label_->SetText(ss.str());
      }

      size_t object_count = GetScene()->GetChildrenCount(true);
      {
        std::stringstream ss;
        ss << "Object count: " << std::fixed << std::setw(6) << std::setprecision(2) << object_count;
        object_count_label_->SetText(ss.str());
      }

      size_t triangle_count = GetScene()->GetTriangleCount();
      {
        std::stringstream ss;
        ss << "Triangle count: " << std::fixed << std::setw(6) << std::setprecision(2) << triangle_count / 1024.0 << "K";
        triangle_count_label_->SetText(ss.str());
      }

      {
        std::stringstream ss;
        ss << "Triangle per sec: " << std::fixed << std::setw(6) << std::setprecision(2) << triangle_count*fps / 1024 / 1024 << "M";
        triangle_per_sec_label_->SetText(ss.str());
      }


      sum_frame_num_ += calls_;
      sum_time_ += accum_time_;
    }

    accum_time_ = calls_ = 0;
  }
}

void FpsDisplay::RemovedFromScene() {
  if (sum_time_ != 0) {
    std::cout << "Average FPS: " << sum_frame_num_ / sum_time_ << std::endl;
  }
}

void FpsDisplay::ScreenResized(size_t /*width*/, size_t height) {
  float scale = 0.25 + (height / 1080.0) * 0.75;
  fps_label_->SetScale(1.5 * scale);
  object_count_label_->SetScale(scale);
  triangle_count_label_->SetScale(scale);
  triangle_per_sec_label_->SetScale(scale);
}

}
