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

#include "game_app.h"

#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/geometry_generator.h"

#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <string>

using namespace tamarindo;

std::unique_ptr<Application> CreateApplication()
{
    return std::make_unique<GameApp>();
}

GameApp::GameApp() : m_WindowProperties("Tamarindo Engine Demo App", 960, 540)
{
    m_WindowProperties.DefaultBackground = {0.1f, 0.1f, 0.1f};
}

bool GameApp::doInitialize()
{
    std::string vertex_shader = R"(
        #version 460 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aUVs;

        uniform mat4 model;
        uniform mat4 viewProj;

        out vec2 UVs;

        void main() {
            gl_Position = viewProj * model * vec4(aPos, 1.0);
            UVs = aUVs;
        }
    )";

    std::string fragment_shader = R"(
        #version 460 core

        in vec2 UVs;

        out vec4 FragColor;

        void main() {
            FragColor = vec4(UVs.x, UVs.y, 0.0f, 1.0f);
        }
    )";

    m_Camera.initialize(glm::vec3(0.0f, 0.0f, 0.0f), -8.f, 8.f, -4.5f, 4.5f,
                        -1.f, 1.f);

    m_SquareMeshTransform.initialize(glm::vec3(0.f, 0.f, 0.f),
                                     glm::vec3(16.f, 9.f, 1.f));

    auto [init, shader_id] =
        ShaderProgram::createNewShader(vertex_shader, fragment_shader);
    if (!init) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }

    m_ShaderProgram = shader_id;

    std::unique_ptr<Mesh> squareMesh = GeometryGenerator::createSquare();

    m_SquareMeshInstance = Mesh::createInstance(*squareMesh.get());

    return true;
}

void GameApp::doTerminate()
{
    Mesh::terminateInstance(m_SquareMeshInstance);
    ShaderProgram::terminateShader(m_ShaderProgram);
}

void GameApp::doUpdate(const Timer& timer)
{
    glm::vec3 direction(0);

    Keyboard* keyboard = g_Keyboard;
    if (keyboard->isKeyPressed(InputKeyCode::D)) {
        direction.x += 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::A)) {
        direction.x -= 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::W)) {
        direction.y += 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::S)) {
        direction.y -= 1.0f;
    }

    if (direction.x != 0 || direction.y != 0) {
        direction = glm::normalize(direction);
        glm::vec3 newPosition = m_Camera.getPosition() +
                                (direction * (float)timer.deltaTime() * 2.f);
        m_Camera.setPosition(newPosition);
    }

    // const double func_frequency = 2.0;
    // const double current_period =
    //     glm::cos(total_time.count() * func_frequency) * 0.5 + 0.5;

    // const double scale_factor = glm::mix(0.5, 0.75, current_period);

    // m_SquareMeshTransform.setScale(
    //     glm::vec3(16.f * scale_factor, 9.f * scale_factor, 1.f));
}

void GameApp::doRender()
{
    ShaderProgram::bindShader(m_ShaderProgram);

    ShaderProgram::setMat4f(m_ShaderProgram, "viewProj",
                            m_Camera.getViewProjectionMatrix());

    ShaderProgram::setMat4f(m_ShaderProgram, "model",
                            m_SquareMeshTransform.getTransformMatrix());

    Mesh::renderMeshInstance(m_SquareMeshInstance);
}
