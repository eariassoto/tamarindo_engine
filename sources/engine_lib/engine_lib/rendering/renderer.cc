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

#include "renderer.h"

#include "logging/logger.h"
#include "rendering/shader_program.h"
#include "world/scene.h"

#include <string>

namespace tamarindo
{
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

// TODO: Fix this hack
std::unique_ptr<ShaderProgram> shader_program = nullptr;
}  // namespace

bool Renderer::initialize()
{
    shader_program =
        ShaderProgram::createNewShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (shader_program == nullptr) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }
    return true;
}


void Renderer::terminate() { shader_program->terminate(); }

void Renderer::tryRenderScene(const Scene& scene)
{
    if (!scene.canRender()) {
        return;
    }

    shader_program->bind();
    scene.bindToShader(*shader_program.get());

    GameObject* game_object = scene.getGameObject();
    game_object->submit(*shader_program.get());
}

}  // namespace tamarindo
