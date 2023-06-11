# Copyright 2023 Emmanuel Arias Soto
find_library(DIRECTX11_LIB d3d11)

add_library(engine_rendering
    buffers.h
    buffers.cc
    #camera_interface.h
    #camera.cc
    #camera.h
    #game_object.cc
    #game_object.h
    #gltf_model.cc
    #gltf_model.h
    #imgui_renderer.cc
    #imgui_renderer.h
    #material.cc
    #material.h
    #mesh.cc
    #mesh.h
    #model.h
    render_state.cc
    render_state.h
    renderer.cc
    renderer.h
    #resources_manager.cc
    #resources_manager.h
    #scene_renderer.cc
    #scene_renderer.h
    #scene.cc
    #scene.h
    solid_color_shader.cc
    solid_color_shader.h
    vertex_input.h
    window_event_handler.h
    window.cc
    window.h "buffers.h" "vertex_input.h")


add_subdirectory(shaders)
add_dependencies(engine_rendering shaders)

target_compile_features(engine_rendering PUBLIC cxx_std_17)

target_include_directories(engine_rendering PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(engine_rendering PUBLIC
  dxgi
  engine_logging
  glm
  imgui
  tinygltf
  ${DIRECTX11_LIB}
)
