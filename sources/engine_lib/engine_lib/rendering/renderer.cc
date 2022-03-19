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
#include "world/scene.h"

namespace tamarindo
{
namespace
{
unsigned int id_counter = 0;
}

void Renderer::registerResources(const std::vector<RenderingAsset*> assets)
{
    for (RenderingAsset* asset : assets) {
        if (Mesh* mesh = dynamic_cast<Mesh*>(asset)) {
            m_Meshes[id_counter++] = mesh;
        } else if (Material* material = dynamic_cast<Material*>(asset)) {
            m_Materials[id_counter++] = material;
        }
    }
}

void Renderer::submitScene(const Scene& scene)
{
    /*if (ICamera* main_camera = scene.getMainCamera()) {
        if (GameObject* game_object = scene.getGameObject()) {
            m_ShaderProgram->bind();

            m_ShaderProgram->setMat4f("viewProj",
                                      main_camera->getViewProjectionMatrix());

            m_ShaderProgram->setMat4f("model",
                                      game_object->getTransform().getMatrix());

            if (const Mesh* mesh = game_object->getMesh()) {
                for (const MeshPrimitive& primitive : mesh->getPrimitives()) {
                    const Material& mat =
    mesh->getMaterial(primitive.getMaterialIndex());
                    mat.submitForRender(*m_ShaderProgram.get());

                    primitive.submitForRender();
                }
            }
        }
    }*/
}

bool Renderer::initialize()
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

        struct MyMaterial {
            vec3 color;
        }; 

        uniform MyMaterial material;

        out vec4 FragColor;

        void main() {
            FragColor = vec4(material.color, 1.0f);
        }
    )";

    m_ShaderProgram =
        ShaderProgram::createNewShaderProgram(vertex_shader, fragment_shader);
    if (m_ShaderProgram == nullptr) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }

    return true;
}

void Renderer::terminate() {
    for (const auto& [k, v]: m_Materials) {
        delete v;
    }

    for (const auto& [k, v] : m_Meshes) {
        delete v;
    }

    m_ShaderProgram->terminate();
}

}  // namespace tamarindo
