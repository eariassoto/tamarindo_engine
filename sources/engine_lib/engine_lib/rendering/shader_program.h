// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include <string>
#include <utility>

namespace tamarindo
{
typedef unsigned int ShaderProgramID;

namespace ShaderProgram
{
[[nodiscard]] std::pair<bool, ShaderProgramID> createNewShader(
    const std::string& vertex_shader_code,
    const std::string& fragment_shader_code);

void bindShader(ShaderProgramID shader_program);
void terminateShader(ShaderProgramID shader_program);

}  // namespace ShaderProgram

}  // namespace tamarindo
