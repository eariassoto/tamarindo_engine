# Copyright 2022 Emmanuel Arias Soto
cmake_minimum_required(VERSION 3.12...3.24)

project(
    TamarindoEngine
    VERSION 1.0.0
    DESCRIPTION "Simple engine for 3D rendering explorations."
    LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()  # (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

include(FetchContent)

# fmt library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.0.0)
FetchContent_MakeAvailable(fmtlib)

# spdlog logging library
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.10.0)
FetchContent_MakeAvailable(spdlog)

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

# imgui library
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/third_party/imgui")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

# Add libraries without CMake support
#   GLAD: Source code generated from https://glad.dav1d.de/
#   IMGUI
add_subdirectory(third_party)

add_subdirectory(engine_lib)

add_subdirectory(tamarindo_editor)
