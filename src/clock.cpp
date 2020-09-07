#include "stopwatch.hpp"
#include <fmt/core.h>
int main() {
  auto th = std::thread([] { fmt::print("Hello World\n"); });
  th.detach();
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
