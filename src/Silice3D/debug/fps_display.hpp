// Copyright (c), Tamas Csala

#ifndef SILICE3D_DEBUG_FPS_DISPLAY_HPP_
#define SILICE3D_DEBUG_FPS_DISPLAY_HPP_

#include <Silice3D/core/game_object.hpp>
#include <Silice3D/gui/label.hpp>

namespace Silice3D {

class FpsDisplay : public GameObject {
 public:
  explicit FpsDisplay(GameObject* parent);

 private:
  constexpr static float kRefreshInterval = 0.1f;

  Label* fps_label_ = nullptr;
  Label* object_count_label_ = nullptr;
  Label* triangle_count_label_ = nullptr;
  Label* triangle_per_sec_label_ = nullptr;
  double sum_frame_num_ = 0.0;
  double sum_time_ = 0.0;
  double accum_time_ = 0.0;
  int calls_ = 0;
  bool first_display_interval_ = true;

  virtual void Update() override;
  virtual void RemovedFromScene() override;
  virtual void ScreenResized(size_t width, size_t height) override;
};

}

#endif
