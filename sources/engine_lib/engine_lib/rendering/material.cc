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

#include "material.h"

#include "rendering/shader_program.h"

#include "glm/glm.hpp"

namespace tamarindo
{
Material::Material(const Color& color) : m_Color(color) {}

void Material::submitForRender(const ShaderProgram& shader_program) const
{
    shader_program.setVec3("material.color", m_Color);
}

void Material::setColor(const Color& color) { m_Color = color; }

Color::Color(int r, int g, int b)
{
    float f_r = static_cast<float>(glm::clamp(r, 0, 255)) / 255;
    float f_g = static_cast<float>(glm::clamp(g, 0, 255)) / 255;
    float f_b = static_cast<float>(glm::clamp(b, 0, 255)) / 255;
    m_Color = glm::vec3(f_r, f_g, f_b);
}

Color::Color(float r, float g, float b)
    : m_Color(glm::clamp(r, 0.0f, 1.0f), glm::clamp(g, 0.0f, 1.0f),
              glm::clamp(b, 0.0f, 1.0f))
{
}

}  // namespace tamarindo
