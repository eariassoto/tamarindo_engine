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
#include "engine_lib/rendering/scene_renderer.h"

#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/gltf_model.h"
#include "engine_lib/world/scene.h"

// TODO: Do not expose this
#include "glad/glad.h"

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

        out vec4 out_color;

        void main() {
            out_color = vec4(material.color, 1.0f);
        }
    )";
}  // namespace

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
    // if (!m_ScenePtr->canRender()) {
    //     return;
    // }

    // GameObject* game_object = m_ScenePtr->getGameObject();
    // const Transform& transform = game_object->getTransform();

    // GLTFModel* model = dynamic_cast<GLTFModel*>(game_object->getModel());
    // if (model == nullptr) {
    //     return;
    // }

    // m_ShaderProgram->bind();
    // m_ScenePtr->bindToShader(*m_ShaderProgram.get());

    // for (const auto& [mesh_index, transforms] : model->m_MeshInstances) {
    //     const GLTFMesh& mesh = model->m_Meshes.at(mesh_index);

    //    for (const auto& primitive : mesh.Primitives) {
    //        glBindVertexArray(primitive.VAO);
    //        for (const auto& t : transforms) {
    //            glm::mat4 model_matrix = transform.getMatrix() *
    //            t.getMatrix(); m_ShaderProgram->setMat4f("model",
    //            model_matrix);

    //            model->m_Materials[primitive.MaterialIndex].submitForRender(
    //                *m_ShaderProgram);
    //            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //            glDrawElements(GL_TRIANGLES, (GLsizei)primitive.IndexCount,
    //                           GL_UNSIGNED_INT, 0);

    //            // TODO: Fix this hack
    //            if (m_RenderWireframe) {
    //                m_DebugMaterial.submitForRender(*m_ShaderProgram);
    //                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //                glDrawElements(GL_TRIANGLES,
    //                (GLsizei)primitive.IndexCount,
    //                               GL_UNSIGNED_INT, 0);
    //            }
    //        }

    //        glBindVertexArray(0);
    //    }
    //}
}

void SceneRenderer::update(const tamarindo::Timer& timer)
{
    // if (g_Keyboard->wasKeyPressedThisFrame(tamarindo::InputKeyCode::Z)) {
    //     m_RenderWireframe = !m_RenderWireframe;
    // }
}

}  // namespace tamarindo
