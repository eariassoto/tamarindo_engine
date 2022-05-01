/*
 Copyright 2022 Emmanuel Arias Soto
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

#pragma once

#include "rendering/shader_program.h"

#include "glm/glm.hpp"

namespace tamarindo
{
// TODO: consider moving it outside of this file
class Color
{
   public:
    Color() = default;
    Color(int r, int g, int b);
    Color(float r, float g, float b);

    operator glm::vec3() const { return m_Color; }

   private:
    glm::vec3 m_Color = glm::vec3(0.0f);
};

class Material
{
   public:
    Material(const Color& color);

    void submitForRender(ShaderProgramID shaderProgram);

    void setColor(const Color& color);

   private:
    Color m_Color;
};

}  // namespace tamarindo
