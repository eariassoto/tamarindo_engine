# Copyright 2022 Emmanuel Arias Soto
# GLAD =======================================================================
add_library(glad
  glad/include/glad/glad.h
  glad/include/KHR/khrplatform.h
  glad/src/glad.c)

target_include_directories(glad PUBLIC glad/include)

# IMGUI (GLFW + OpenGL3) =====================================================

set(IMGUI_DIR imgui)

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
  "${IMGUI_DIR}/imgui_demo.cpp"
  "${IMGUI_DIR}/imgui_draw.cpp"
  "${IMGUI_DIR}/imgui_tables.cpp"
  "${IMGUI_DIR}/imgui_widgets.cpp"
  "${IMGUI_DIR}/imgui.cpp"
  
  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
  "${IMGUI_DIR}/backends/imgui_impl_glfw.h")

add_library(imgui STATIC ${SOURCE_LIST})

target_link_libraries(imgui PRIVATE glfw)

target_include_directories(imgui
  PUBLIC ${IMGUI_DIR}
  PUBLIC ${IMGUI_DIR}/backends)
