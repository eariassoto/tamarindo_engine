# Copyright 2023 Emmanuel Arias Soto
add_library(engine_utils
    macros.h
    timer.cc
    timer.h
)

target_include_directories(engine_utils PUBLIC ${CMAKE_SOURCE_DIR})
