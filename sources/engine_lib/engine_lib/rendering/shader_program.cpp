// Copyright(c) 2021-2022 Emmanuel Arias
#include "shader_program.h"

#include "logging/logger.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

namespace tamarindo
{
namespace
{
std::pair<bool, unsigned int> tryCompileShader(const char* shader_code,
                                               GLenum shader_type)
{
    int success;
    char info_log[512];

    unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);

    // print compile errors if any
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        TM_LOG_ERROR("Shader {} compilation failed: {}", shader_type, info_log);
        return {false, 0};
    };
    return {true, shader_id};
}
}  // namespace

namespace ShaderProgram
{
std::pair<bool, ShaderProgramID> createNewShader(
    const std::string& vertex_shader_code,
    const std::string& fragment_shader_code)
{
    auto [v_compiled, vertex_shader_id] =
        tryCompileShader(vertex_shader_code.c_str(), GL_VERTEX_SHADER);
    if (!v_compiled) {
        return {false, 0};
    }

    auto [fCompiled, frag_shader_id] =
        tryCompileShader(fragment_shader_code.c_str(), GL_FRAGMENT_SHADER);
    if (!fCompiled) {
        glDeleteShader(vertex_shader_id);
        return {false, 0};
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

        return {false, 0};
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(frag_shader_id);

    return {true, shader_program_id};
}

void setInt(ShaderProgramID shader, const std::string& name,
                           int value)
{
    unsigned int transformLoc = glGetUniformLocation(shader, name.c_str());
    glUniform1i(transformLoc, value);
}

void setFloat(ShaderProgramID shader, const std::string& name,
                             float value)
{
    unsigned int transformLoc = glGetUniformLocation(shader, name.c_str());
    glUniform1f(transformLoc, value);
}

void setMat4f(ShaderProgramID shader, const std::string& name,
                             const glm::mat4& value)
{
    unsigned int transformLoc = glGetUniformLocation(shader, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void setVec3(ShaderProgramID shader, const std::string& name,
                            const glm::vec3& value)
{
    unsigned int transformLoc = glGetUniformLocation(shader, name.c_str());
    glUniform3fv(transformLoc, 1, glm::value_ptr(value));
}

void bindShader(ShaderProgramID shader_program)
{
    glUseProgram(shader_program);
}

void terminateShader(ShaderProgramID shader_program)
{
    glDeleteProgram(shader_program);
}

}  // namespace ShaderProgram

}  // namespace tamarindo
