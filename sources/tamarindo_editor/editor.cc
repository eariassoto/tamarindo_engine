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

#include "editor.h"

#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/world/camera.h"

#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <string>

using namespace tamarindo;

namespace
{
static constexpr float SQUARE_VERT[20] = {
    +0.5f, +0.5f, 0.0f, 1.0f, 0.0f,  // top right
    +0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // bottom left
    -0.5f, +0.5f, 0.0f, 0.0f, 0.0f   // top left
};

static constexpr unsigned int SQUARE_IND[6] = {
    0, 1, 3,  // first triangle
    1, 2, 3,  // second triangle
};

}  // namespace

std::unique_ptr<Application> CreateApplication()
{
    return std::make_unique<Editor>();
}

Editor::Editor() : m_WindowProperties("Tamarindo Editor", 960, 540)
{
    m_WindowProperties.DefaultBackground = {0.1f, 0.1f, 0.1f};
}

bool Editor::doInitialize()
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

    SphericalCameraParams sphericalCameraParams;
    sphericalCameraParams.FovAngleInRad = glm::radians(45.f);
    // TODO: Get from Application
    sphericalCameraParams.AspectRatio = 960.f / 540;
    sphericalCameraParams.ZNear = 0.1f;
    sphericalCameraParams.ZFar = 100.f;
    sphericalCameraParams.SpherePosition = glm::vec3(0.0f);
    sphericalCameraParams.SphereRadius = 5.f;
    sphericalCameraParams.MoveRadiusUnitsPerSecond = 5.f;
    sphericalCameraParams.SpeedRadsPerSec = glm::radians(50.f);

    m_Camera = std::make_unique<SphericalCamera>(sphericalCameraParams);

    m_SquareMeshTransform.initialize(glm::vec3(0.f, 0.f, 0.f),
                                     glm::vec3(16.f, 9.f, 1.f));

    auto [init, shader_id] =
        ShaderProgram::createNewShader(vertex_shader, fragment_shader);
    if (!init) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }

    m_ShaderProgram = shader_id;

    m_SquareMesh = std::make_unique<Mesh>(1);

    std::vector<float> vertices(
        SQUARE_VERT,
        SQUARE_VERT + sizeof(SQUARE_VERT) / sizeof(SQUARE_VERT[0]));
    std::vector<unsigned int> indices(
        SQUARE_IND, SQUARE_IND + sizeof(SQUARE_IND) / sizeof(SQUARE_IND[0]));
    m_SquareMesh->addPrimitive(vertices, indices);

    if (!m_SquareMesh->initialize()) {
        return false;
    }

    return true;
}

void Editor::doTerminate()
{
    m_SquareMesh->terminate();
    ShaderProgram::terminateShader(m_ShaderProgram);
}

void Editor::doUpdate(const Timer& timer) { m_Camera->onUpdate(timer); }

void Editor::doRender()
{
    ShaderProgram::bindShader(m_ShaderProgram);

    ShaderProgram::setMat4f(m_ShaderProgram, "viewProj",
                            m_Camera->getViewProjectionMatrix());

    ShaderProgram::setMat4f(m_ShaderProgram, "model",
                            m_SquareMeshTransform.getTransformMatrix());
    m_SquareMesh->submit();
}
