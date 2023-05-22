# Copyright 2023 Emmanuel Arias Soto
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

add_library(engine_logging
    logger.cc
)

target_include_directories(engine_logging PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(engine_logging PUBLIC
  fmt
  spdlog
)
