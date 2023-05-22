# Copyright 2022-2023 Emmanuel Arias Soto
cmake_minimum_required(VERSION 3.12...3.24)

project(
    TamarindoEngine
    VERSION 1.0.0
    DESCRIPTION "Simple engine for 3D rendering explorations."
    LANGUAGES C CXX)

include(FetchContent)

# GLM math library
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)

# GLFW library
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8)
FetchContent_MakeAvailable(glfw)

# tinygltf library
FetchContent_Declare(
  tinygltf
  GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
  GIT_TAG v2.6.3)
FetchContent_MakeAvailable(tinygltf)

# Declare ImGui as an external project
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

# Add libraries without CMake support
#   GLAD: Source code generated from https://glad.dav1d.de/
#   IMGUI
add_subdirectory(third_party)

add_subdirectory(engine_lib)

add_subdirectory(tamarindo_editor)
