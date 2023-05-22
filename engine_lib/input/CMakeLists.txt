# Copyright 2023 Emmanuel Arias Soto
add_library(engine_input
    input_defs.h
    input_manager.cc
    input_manager.h
    keyboard.cc
    keyboard.h)

target_include_directories(engine_input PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(engine_input PUBLIC
  engine_logging
  glfw
)
