cmake_minimum_required(VERSION 3.0)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
set(OpenGL_GL_PREFERENCE GLVND)
project(imgui)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui src/imgui.cpp src/imgui_demo.cpp src/imgui_draw.cpp src/imgui_widgets.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE: include ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE
        OpenGL::GL
        glfw
        ${GLEW_LIBRARIES})

