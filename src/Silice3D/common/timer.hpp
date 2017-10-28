// Copyright (c) Tamas Csala

#ifndef SILICE3D_COMMON_TIMER_HPP_
#define SILICE3D_COMMON_TIMER_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Silice3D {

class Timer {
 public:
  double Tick();
  void Stop();
  void Start();
  void Toggle();

  double GetCurrentTime() const { return current_time_; }
  double GetDeltaTime() const { return dt_; }

 private:
  double last_time_ = 0.0, current_time_ = 0.0, dt_ = 0.0;
  bool stopped_ = false;
};

}  // namespace Silice3D

#endif
