/*
 Copyright 2021-2022 Emmanuel Arias Soto

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "engine_lib/rendering/shader_program.h"

#include "engine_lib/logging/logger.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

namespace tamarindo
{
namespace
{
bool tryCompileShader(unsigned int* shader_id, const char* shader_code,
                      GLenum shader_type)
{
    int success;
    char info_log[512];

    *shader_id = glCreateShader(shader_type);
    glShaderSource(*shader_id, 1, &shader_code, NULL);
    glCompileShader(*shader_id);

    // print compile errors if any
    glGetShaderiv(*shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*shader_id, 512, NULL, info_log);
        TM_LOG_ERROR("Shader {} compilation failed: {}", shader_type, info_log);
        return false;
    };
    return true;
}
}  // namespace

std::unique_ptr<ShaderProgram> ShaderProgram::createNewShaderProgram(
    const std::string& vertex_shader_code,
    const std::string& fragment_shader_code)
{
    unsigned int vertex_shader_id = 0;
    if (!tryCompileShader(&vertex_shader_id, vertex_shader_code.c_str(),
                          GL_VERTEX_SHADER)) {
        return nullptr;
    }

    unsigned int frag_shader_id = 0;
    if (!tryCompileShader(&frag_shader_id, fragment_shader_code.c_str(),
                          GL_FRAGMENT_SHADER)) {
        glDeleteShader(vertex_shader_id);
        return nullptr;
    }

    // Create shader program
    unsigned int shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, frag_shader_id);
    glLinkProgram(shader_program_id);

    // print linking errors if any
    int success;
    char info_log[512];
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
        TM_LOG_ERROR("Shader program failed to link: {}, infoLog");

        return nullptr;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(frag_shader_id);

    return std::unique_ptr<ShaderProgram>(new ShaderProgram(shader_program_id));
}

ShaderProgram::ShaderProgram(unsigned int shader_program_id)
    : m_Id(shader_program_id)
{
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    unsigned int transformLoc = glGetUniformLocation(m_Id, name.c_str());
    glUniform1i(transformLoc, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    unsigned int transformLoc = glGetUniformLocation(m_Id, name.c_str());
    glUniform1f(transformLoc, value);
}

void ShaderProgram::setMat4f(const std::string& name,
                             const glm::mat4& value) const
{
    unsigned int transformLoc = glGetUniformLocation(m_Id, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value)const
{
    unsigned int transformLoc = glGetUniformLocation(m_Id, name.c_str());
    glUniform3fv(transformLoc, 1, glm::value_ptr(value));
}

void ShaderProgram::bind() { glUseProgram(m_Id); }

void ShaderProgram::terminate() { glDeleteProgram(m_Id); }

}  // namespace tamarindo
