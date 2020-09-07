#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
class stopwatch {
public:
  stopwatch();
  void start() noexcept;
  void stop() noexcept;
  void reset() noexcept;
  void update();
  ~stopwatch();

private:
  std::chrono::steady_clock::time_point m_start;
  std::chrono::steady_clock::time_point m_end;
  GLFWwindow *window;
  size_t milliseconds;
};
