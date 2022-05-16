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

// define only once
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "gltf_mesh.h"

#include "logging/logger.h"
#include "rendering/shader_program.h"

#include "glad/glad.h"

#include <cassert>

namespace tamarindo
{
GLTFModel::GLTFModel(const tinygltf::Model& model) : m_Model(model) {}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

void CheckOpenGLErr()
{
    const GLenum err = glGetError();
    if (GL_NO_ERROR != err) TM_LOG_ERROR("GL Error: {}", err);
}

void GLTFModel::bindMesh(int mesh_index)
{
    if (m_Meshes.find(mesh_index) != m_Meshes.end()) {
        return;
    }

    const tinygltf::Mesh& mesh = m_Model.meshes[mesh_index];
    TM_LOG_INFO("Processing mesh {}: {}", mesh_index, mesh.name);
    GLTFMesh gltf_mesh;

    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];

        GLuint vao = -1;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        for (auto& [name, buffer_view_index] : primitive.attributes) {
            tinygltf::Accessor accessor = m_Model.accessors[buffer_view_index];

            const tinygltf::BufferView& buffer_view =
                m_Model.bufferViews[accessor.bufferView];
            int byte_stride = accessor.ByteStride(buffer_view);
            int size = accessor.type == TINYGLTF_TYPE_SCALAR
                           ? 1
                           : size = accessor.type;

            const bool is_normalized = accessor.normalized ? GL_TRUE : GL_FALSE;
            int attr_location = -1;
            const int vb_binding_index = 0;
            if (name.compare("POSITION") == 0) {
                attr_location = 0;
            } else if (name.compare("TEXCOORD_0") == 0) {
                attr_location = -1;
            }

            if (attr_location != -1) {
                TM_LOG_INFO("For attribute {}, location = {}", name,
                            attr_location);

                glEnableVertexArrayAttrib(vao, attr_location);
                glVertexArrayAttribBinding(vao, attr_location,
                                           vb_binding_index);
                glVertexArrayAttribFormat(vao, attr_location, size,
                                          accessor.componentType, is_normalized,
                                          (GLuint)accessor.byteOffset);

                glVertexArrayVertexBuffer(vao, attr_location,
                                          m_Buffers[buffer_view_index],
                                          accessor.byteOffset, byte_stride);
            }
        }

        tinygltf::Accessor index_accessor =
            m_Model.accessors[primitive.indices];
        glVertexArrayElementBuffer(vao, m_Buffers[index_accessor.bufferView]);
        CheckOpenGLErr();

        gltf_mesh.Primitives.push_back(
            GLTFPrimitive{vao, index_accessor.count, primitive.material});
    }
    m_Meshes[mesh_index] = gltf_mesh;
}

void GLTFModel::bindModelNodes(int node_index)
{
    assert((node_index >= 0) && (node_index < m_Model.nodes.size()));
    const tinygltf::Node& node = m_Model.nodes[node_index];
    TM_LOG_INFO("Processing node {}: {}", node_index, node.name);
    if ((node.mesh >= 0) && (node.mesh < m_Model.meshes.size())) {
        Transform t;
        if (node.translation.size() == 3) {
            t.setPosition(glm::vec3((float)node.translation.at(0),
                                    (float)node.translation.at(1),
                                    (float)node.translation.at(2)));
        }
        if (node.scale.size() == 3) {
            t.setScale(glm::vec3((float)node.scale.at(0),
                                 (float)node.scale.at(1),
                                 (float)node.scale.at(2)));
        }
        if (node.rotation.size() == 4) {
            t.setRotation(glm::quat(
                (float)node.rotation.at(3), (float)node.rotation.at(0),
                (float)node.rotation.at(1), (float)node.rotation.at(2)));
        }
        bindMesh(node.mesh);
        m_MeshInstances[node.mesh].push_back(t);
    }

    for (const int child_node_index : node.children) {
        bindModelNodes(child_node_index);
    }
}

bool GLTFModel::initialize()
{
    for (size_t i = 0; i < m_Model.bufferViews.size(); ++i) {
        const tinygltf::BufferView& buffer_view = m_Model.bufferViews[i];

        if (buffer_view.target != GL_ELEMENT_ARRAY_BUFFER &&
            buffer_view.target != GL_ARRAY_BUFFER) {
            continue;
        }

        unsigned int vbo = 0;
        glCreateBuffers(1, &vbo);
        m_Buffers[i] = vbo;

        const tinygltf::Buffer& buffer = m_Model.buffers[buffer_view.buffer];
        glNamedBufferData(vbo, buffer_view.byteLength,
                          &buffer.data.at(0) + buffer_view.byteOffset,
                          GL_STATIC_DRAW);

        CheckOpenGLErr();

        TM_LOG_INFO(
            "Buffer view: {}, buffer.data.size = {}, "
            "bufferview.byteOffset = {}",
            i, buffer.data.size(), buffer_view.byteOffset);
    }

    for (const tinygltf::Material& mat : m_Model.materials) {
        auto& base_color = mat.pbrMetallicRoughness.baseColorFactor;
        m_Materials.emplace_back(Material(Color(
            (float)base_color[0], (float)base_color[1], (float)base_color[2])));
    }

    const tinygltf::Scene& scene = m_Model.scenes[m_Model.defaultScene];
    for (const int node_index : scene.nodes) {
        bindModelNodes(node_index);
    }

    return true;
}

void GLTFModel::submit(const ShaderProgram& shader_program)
{
    for (const auto& [mesh_index, transforms] : m_MeshInstances) {
        const GLTFMesh& mesh = m_Meshes.at(mesh_index);

        for (const auto& primitive : mesh.Primitives) {
            m_Materials[primitive.MaterialIndex].submitForRender(
                shader_program);

            glBindVertexArray(primitive.VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            for (const auto& t : transforms) {
                shader_program.setMat4f("meshTransform", t.getMatrix());
                glDrawElements(GL_TRIANGLES, (GLsizei)primitive.IndexCount,
                               GL_UNSIGNED_INT, 0);
            }

            m_DebugMaterial.submitForRender(shader_program);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            for (const auto& t : transforms) {
                shader_program.setMat4f("meshTransform", t.getMatrix());
                glDrawElements(GL_TRIANGLES, (GLsizei)primitive.IndexCount,
                               GL_UNSIGNED_INT, 0);
            }

            glBindVertexArray(0);
        }
    }
}

void GLTFModel::terminate()
{
    for (auto [key, value] : m_Buffers) {
        glDeleteBuffers(1, &value);
    }
}

}  // namespace tamarindo
