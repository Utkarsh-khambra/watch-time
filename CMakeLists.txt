
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# Note: update this to your new project's name and version
project(
  clock
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

find_package(fmt CONFIG REQUIRED)
# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.

# ---- Create library ----
add_subdirectory(imgui)
add_executable(clock src/main.cpp src/stopwatch.cpp)
# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(clock INTERFACE) set_target_properties(clock PROPERTIES
# INTERFACE_COMPILE_FEATURES cxx_std_17)

target_compile_features(clock PUBLIC cxx_std_20)
target_compile_options(clock PRIVATE -ggdb  -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused )

target_include_directories(
  clock PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include imgui/include imgui>
)

target_link_libraries(clock PRIVATE fmt::fmt imgui)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
