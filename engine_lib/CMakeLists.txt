# Copyright 2022 Emmanuel Arias Soto
file(GLOB HEADER_LIST CONFIGURE_DEPENDS
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/core/*.h"
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/input/*.h"
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/logging/*.h"
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/rendering/*.h"
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/utils/*.h"
  "${TamarindoEngine_SOURCE_DIR}/include/engine_lib/world/*.h")

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "core/*.cc"
  "input/*.cc"
  "logging/*.cc"
  "rendering/*.cc"
  "world/*.cc")

add_library(engine_lib ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(engine_lib PUBLIC ../include ${glad_SOURCE_DIR}/include)

target_link_libraries(engine_lib PUBLIC
  fmt
  glad
  glfw
  glm
  imgui
  spdlog
  tinygltf)

target_compile_features(engine_lib PUBLIC cxx_std_17)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
