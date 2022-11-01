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

#ifndef ENGINE_LIB_RENDERING_SHADER_PROGRAM_H_
#define ENGINE_LIB_RENDERING_SHADER_PROGRAM_H_

#include "glm/glm.hpp"

#include <memory>
#include <string>

namespace tamarindo
{
class ShaderProgram
{
   public:
    ShaderProgram() = delete;

    static std::unique_ptr<ShaderProgram> createNewShaderProgram(
        const std::string &vertex_shader_code,
        const std::string &fragment_shader_code);

    void bind();
    void terminate();

    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4f(const std::string &name, const glm::mat4 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;

   private:
    ShaderProgram(unsigned int shader_program_id);
    unsigned int m_Id = 0;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_SHADER_PROGRAM_H_
