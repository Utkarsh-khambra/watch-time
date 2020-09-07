#include "stopwatch.hpp"
#include <fmt/core.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

// Here wait is the amount of time for which stopwatch was stopped
static inline std::string
get_time_string(std::chrono::steady_clock::time_point start,
                std::chrono::steady_clock::time_point end, long wait) noexcept {
  auto milli =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count() -
      wait;
  auto seconds = milli / 1000;
  milli = milli % 1000;
  if (auto minutes = seconds / 60; minutes) {
    seconds = seconds % 60;
    if (auto hours = minutes / 60; hours) {
      minutes = minutes % 60;
      return fmt::format("{}h:{}m:{}s:{}ms", hours, minutes, seconds, milli);
    }
    return fmt::format("{}m:{}s:{}ms", minutes, seconds, milli);
  }
  return fmt::format("{}s:{}ms", seconds, milli);
}
static inline void show_time(std::chrono::steady_clock::time_point start,
                             std::chrono::steady_clock::time_point end,
                             long wait) {
  auto print = get_time_string(start, end, wait);
  ImGui::Indent(220);
  ImGui::Text(print.c_str());
  ImGui::Unindent(220);
  ImGui::NewLine();
}

static void setup_imgui(GLFWwindow *window, const char *glsl_version) noexcept {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
  // Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);
}

stopwatch::stopwatch() {
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  if (!glfwInit())
    throw std::runtime_error("GLFW not initialized");
  window = glfwCreateWindow(600, 300, "clock", NULL, NULL);
  if (window == NULL)
    throw std::runtime_error("Cannot create window");
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync
  bool err = glewInit();
  if (err) {
    throw std::runtime_error("Cannot initialize glew");
  }
  setup_imgui(window, glsl_version);
}
void stopwatch::start() noexcept { m_start = std::chrono::steady_clock::now(); }
void stopwatch::stop() noexcept { m_end = std::chrono::steady_clock::now(); }
void stopwatch::reset() noexcept {
  m_end = m_start = std::chrono::steady_clock::now();
}

// Stopwatch starts in a reseted condition
void stopwatch::update() {
  bool started = false;
  bool reseted = true;
  bool stopped = false;

  long wait = 0;
  auto wait_start = std::chrono::steady_clock::now();
  int screen_width, screen_height;
  int win_x, win_y;
  auto &io = ImGui::GetIO();
  io.Fonts->AddFontDefault();
  auto *font = io.Fonts->AddFontFromFileTTF(
      "/home/utkarsh/watch-time/assets/Lexend_Peta/LexendPeta-Regular.ttf", 50);
  glfwGetFramebufferSize(window, &screen_width, &screen_height);

  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application. Generally you may always pass all inputs
    // to dear imgui, and hide them from your application based on those two
    // flags.
    glfwGetWindowPos(window, &win_x, &win_y);
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(win_x, win_y));
    ImGui::SetNextWindowSize(ImVec2(screen_width, screen_height));
    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    {

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
      ImGui::PushFont(font);
      started ? show_time(m_start, std::chrono::steady_clock::now(), wait)
              : (stopped ? show_time(m_start, m_end, wait)
                         : show_time(m_start, m_start, wait));
      ImGui::PopFont();

      if (ImGui::Button("Start", ImVec2(80, 40)) and (!started)) {
        // Because only if reseted set the starting point of clock to a new
        // timepoint
        if (reseted)
          start();
        // This branch is executed only when there is some wait, which is only
        // possible for clocks with current state as stopped and reseted
        if (stopped and (!reseted)) {
          wait += std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::steady_clock::now() - wait_start)
                      .count();
          // wait_start = std::chrono::steady_clock::now();
        }
        started = true;
        stopped = false;
        reseted = false;
      }
      ImGui::SameLine();
      ImGui::Indent(105);

      // This branch sets both reseted and stopped because gui of stopwatch both
      // resets the clock also stops it or puts the clock in its intialzed
      // settings
      if (ImGui::Button("Reset", ImVec2(80, 40)) and (!reseted)) {
        reset();
        started = false;
        stopped = true;
        reseted = true;
        wait = 0;
      }
      ImGui::SameLine();
      ImGui::Indent(105);
      if (ImGui::Button("Stop", ImVec2(80, 40)) and (!stopped)) {
        // Wait should counted only when a started stopwatch has been stopped
        stop();
        wait_start = std::chrono::steady_clock::now();

        started = false;
        stopped = true;
        reseted = false;
      }
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }
}

stopwatch::~stopwatch() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}
