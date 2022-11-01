# Copyright 2022 Emmanuel Arias Soto
add_executable(tamarindo_editor
    editor.cc
    editor.h
    main.cc)
target_compile_features(tamarindo_editor PRIVATE cxx_std_17)

target_link_libraries(tamarindo_editor PRIVATE engine_lib)
