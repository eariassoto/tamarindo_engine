# Copyright 2023 Emmanuel Arias Soto
add_custom_target(shaders)

# Vertex shaders
set(HLSL_SHADER_FILES_VS
    solid_color_vs.hlsl
)
foreach(FILE ${HLSL_SHADER_FILES_VS})
    set_source_files_properties(${FILE} PROPERTIES ShaderType "vs")
    set_source_files_properties(${FILE} PROPERTIES ShaderModel "4_0")
endforeach()

# Pixel shaders
set(HLSL_SHADER_FILES_PS
    solid_color_ps.hlsl
)
foreach(FILE ${HLSL_SHADER_FILES_PS})
    set_source_files_properties(${FILE} PROPERTIES ShaderType "ps")
    set_source_files_properties(${FILE} PROPERTIES ShaderModel "4_0")
endforeach()

set(HLSL_SHADER_FILES ${HLSL_SHADER_FILES_VS} ${HLSL_SHADER_FILES_PS})

foreach(FILE ${HLSL_SHADER_FILES})
  get_filename_component(FILE_WE ${FILE} NAME_WE)
  get_source_file_property(shadertype ${FILE} ShaderType)
  get_source_file_property(shadermodel ${FILE} ShaderModel)
  add_custom_command(TARGET shaders
                     COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
                     MAIN_DEPENDENCY ${FILE}
                     COMMENT "HLSL ${FILE}"
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     VERBATIM)
endforeach(FILE)
