// Copyright(c) 2021-2022 Emmanuel Arias
#pragma once

#include "glm/glm.hpp"

#include <string>
#include <utility>

namespace tamarindo
{
typedef unsigned int ShaderProgramID;

namespace ShaderProgram
{
[[nodiscard]] std::pair<bool, ShaderProgramID> createNewShader(
    const std::string &vertex_shader_code,
    const std::string &fragment_shader_code);

void bindShader(ShaderProgramID shader_program);
void terminateShader(ShaderProgramID shader_program);

void setInt(ShaderProgramID shader, const std::string &name, int value);
void setFloat(ShaderProgramID shader, const std::string &name, float value);
void setMat4f(ShaderProgramID shader, const std::string &name,
              const glm::mat4 &value);
void setVec3(ShaderProgramID shader, const std::string &name,
             const glm::vec3 &value);

}  // namespace ShaderProgram

}  // namespace tamarindo
