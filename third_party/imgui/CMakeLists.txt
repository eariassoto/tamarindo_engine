# Copyright 2022-2023 Emmanuel Arias Soto
# IMGUI (GLFW + OpenGL3) =====================================================

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imgui.cpp")
  
  #"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
  #"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
  #"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  #"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h")

add_library(imgui STATIC ${SOURCE_LIST})

target_include_directories(imgui
  PUBLIC ${imgui_SOURCE_DIR}
  PUBLIC ${imgui_SOURCE_DIR}/backends)
