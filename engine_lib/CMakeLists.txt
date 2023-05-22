# Copyright 2022-2023 Emmanuel Arias Soto
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
  "engine_lib/core/*.h"
  "engine_lib/input/*.h"
  "engine_lib/rendering/*.h"
  "engine_lib/utils/*.h"
  "engine_lib/world/*.h")

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "core/*.cc"
  "input/*.cc"
  "rendering/*.cc"
  "world/*.cc")

add_library(engine_lib ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(engine_lib PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${glad_SOURCE_DIR}/include)

add_subdirectory(logging)

target_link_libraries(engine_lib PUBLIC
  glad
  glfw
  glm
  imgui
  tinygltf
  engine_logging)

target_compile_features(engine_lib PUBLIC cxx_std_17)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
