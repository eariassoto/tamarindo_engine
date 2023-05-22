# Copyright 2022 Emmanuel Arias Soto
add_executable(tamarindo_editor
    application.cc
    application.h
    main.cc)

target_include_directories(tamarindo_editor PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_features(tamarindo_editor PRIVATE cxx_std_17)

target_link_libraries(tamarindo_editor PRIVATE
    engine_logging
    engine_rendering
    engine_input
    engine_utils)
