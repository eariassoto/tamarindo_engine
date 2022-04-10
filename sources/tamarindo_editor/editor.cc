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
#include "engine_lib/rendering/material.h"
#include "engine_lib/world/camera.h"

#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"

#include <string>

using namespace tamarindo;

namespace
{
// clang-format off
static constexpr unsigned int CUBE_VERTEX_DATA_SIZE = 6 * 5;
static constexpr unsigned int CUBE_PRIMITIVES = 6;
static constexpr float CUBE_VERTICES[CUBE_PRIMITIVES*CUBE_VERTEX_DATA_SIZE] = {
    // positions          // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f
};
// clang-format on

static const Material g_MATERIALS[CUBE_PRIMITIVES] = {
    Material(Color(0, 155, 72)),  Material(Color(255, 255, 255)),
    Material(Color(183, 18, 52)), Material(Color(255, 213, 0)),
    Material(Color(0, 70, 173)),  Material(Color(255, 88, 0))};

}  // namespace

std::unique_ptr<Application> CreateApplication()
{
    return std::make_unique<Editor>();
}

bool Editor::doInitialize()
{
    std::string vertex_shader = R"(
        #version 460 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aUVs;

        uniform mat4 model;
        uniform mat4 viewProj;

        void main() {
            gl_Position = viewProj * model * vec4(aPos, 1.0);
        }
    )";

    std::string fragment_shader = R"(
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

    auto camera = std::make_unique<SphericalCamera>(sphericalCameraParams);

    auto [init, shader_id] =
        ShaderProgram::createNewShader(vertex_shader, fragment_shader);
    if (!init) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }

    m_ShaderProgram = shader_id;

    auto cube_mesh = std::make_unique<Mesh>(1);

    std::vector<unsigned int> indices;
    indices.resize(6);
    for (unsigned int i = 0; i < 6; i++) {
        indices[i] = i;
    }

    for (unsigned int i = 0; i < 6; i++) {
        cube_mesh->addPrimitive(CUBE_VERTICES + (CUBE_VERTEX_DATA_SIZE * i),
                                CUBE_VERTEX_DATA_SIZE, indices.data(),
                                (unsigned int)indices.size(), g_MATERIALS[i]);
    }

    if (!cube_mesh->initialize()) {
        return false;
    }

    auto cube_game_object = std::make_unique<GameObject>(
        Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)),
        std::move(cube_mesh));

    m_MainScene = std::make_unique<Scene>();
    m_MainScene->setGameObject(std::move(cube_game_object));
    m_MainScene->setCamera(std::move(camera));

    return true;
}

bool Editor::doPreInitialize() { return true; }

std::unique_ptr<ApplicationProperties> Editor::loadApplicationProperties()
{
    auto props = std::make_unique<ApplicationProperties>();

    props->setWindowDefaultBackground({0.1f, 0.1f, 0.1f, 1.0f});
    props->setWindowSize(960, 540);
    props->setWindowTitle("Tamarindo Editor");

    return props;
}

void Editor::doTerminate()
{
    m_MainScene->terminate();
    ShaderProgram::terminateShader(m_ShaderProgram);
}

void Editor::doUpdate(const Timer& timer) { m_MainScene->update(timer); }

void Editor::doRender()
{
    if (m_MainScene == nullptr) {
        return;
    }

    ICamera* camera = m_MainScene->getCamera();
    GameObject* game_object = m_MainScene->getGameObject();

    if (camera == nullptr || game_object == nullptr) {
        return;
    }
    ShaderProgram::bindShader(m_ShaderProgram);

    ShaderProgram::setMat4f(m_ShaderProgram, "viewProj",
                            camera->getViewProjectionMatrix());

    ShaderProgram::setMat4f(m_ShaderProgram, "model",
                            game_object->getTransform().getMatrix());

    game_object->getMesh()->submit(m_ShaderProgram);
}
