# Copyright 2023 Emmanuel Arias Soto
add_library(engine_logging
    logger.cc
)

target_compile_features(engine_logging PUBLIC cxx_std_17)

target_include_directories(engine_logging PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(engine_logging PUBLIC
  fmt
  spdlog
)
