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
#include "scene_renderer.h"

#include "scene.h"

#include "engine_lib/logging/logger.h"

#include <string>

namespace
{
std::string VERTEX_SHADER = R"(
        #version 460 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aUVs;

        uniform mat4 model;
        uniform mat4 viewProj;

        void main() {
            gl_Position = viewProj * model * vec4(aPos, 1.0);
        }
    )";

std::string FRAGMENT_SHADER = R"(
        #version 460 core

        struct MyMaterial {
            vec3 color;
        };
        uniform MyMaterial material;

        out vec4 FragColor;

        void main() {
            FragColor = vec4(material.color, 1.0f);
        }
    )";
}  // namespace

using namespace tamarindo;

SceneRenderer::SceneRenderer(Scene* scene_ptr)
    : m_ScenePtr(scene_ptr)
{
}

bool SceneRenderer::initialize()
{
    m_ShaderProgram =
        ShaderProgram::createNewShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (m_ShaderProgram == nullptr) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }
    return true;
}

void SceneRenderer::terminate() { m_ShaderProgram->terminate(); }

void SceneRenderer::render()
{
    if (!m_ScenePtr->canRender()) {
        return;
    }

    m_ShaderProgram->bind();
    m_ScenePtr->bindToShader(*m_ShaderProgram.get());

    GameObject* game_object = m_ScenePtr->getGameObject();
    game_object->submit(*m_ShaderProgram.get());
}
