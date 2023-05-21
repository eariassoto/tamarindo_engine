/*
 Copyright 2021-2023 Emmanuel Arias Soto

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
#include "engine_lib/rendering/gltf_model.h"
#include "engine_lib/rendering/material.h"
#include "engine_lib/rendering/mesh.h"
#include "engine_lib/world/camera.h"

#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "imgui.h"

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

Editor::Editor() = default;

Editor::~Editor() = default;

bool Editor::doInitialize()
{
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

    GLTFGameObjectDesc desc;
    desc.ModelPath = "../../third_party/kenney/carkit/Models/glTF/race.glb";

    std::unique_ptr<GameObject> game_object = GLTFGameObjectLoader::load(desc);

    /* auto gltf_mesh = std::make_unique<GLTFModel>(model);
     gltf_mesh->initialize();*/

    auto camera = std::make_unique<SphericalCamera>(sphericalCameraParams);

    /* auto cube_game_object = std::make_unique<GameObject>(
         Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)),
         std::move(gltf_mesh));*/

    m_MainScene = std::make_unique<Scene>();
    m_MainScene->setGameObject(std::move(game_object));
    m_MainScene->setCamera(std::move(camera));

    return true;
}

bool Editor::doPreInitialize() { return true; }

void Editor::doTerminate()
{
    if (m_MainScene != nullptr) {
        m_MainScene->terminate();
        m_MainScene.reset();
    }
}

void Editor::doUpdate(const Timer& timer) { m_MainScene->update(timer); }
