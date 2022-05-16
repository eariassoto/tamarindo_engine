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

#include "mesh.h"

#include "rendering/shader_program.h"

#include <glad/glad.h>

namespace tamarindo
{
Mesh::Mesh(unsigned int primitive_count)
{
    m_Primitives.reserve(primitive_count);
}

void Mesh::addPrimitive(const float* vertex_data, unsigned int vertex_data_size,
                        const unsigned int* index_data,
                        unsigned int index_data_size, const Material& material)
{
    m_Primitives.emplace_back(vertex_data, vertex_data_size, index_data,
                              index_data_size, material);
}

bool Mesh::initialize()
{
    for (Primitive& p : m_Primitives) {
        if (!p.initialize()) {
            return false;
        }
    }

    return true;
}

void Mesh::terminate()
{
    for (Primitive& p : m_Primitives) {
        p.terminate();
    }
}

void Mesh::submit(const ShaderProgram& shader_program)
{
    for (Primitive& p : m_Primitives) {
        p.submit(shader_program);
    }
}

Mesh::Primitive::Primitive(const float* vertex_data,
                           unsigned int vertex_data_size,
                           const unsigned int* index_data,
                           unsigned int index_data_size,
                           const Material& material)
    : m_IndexDataSize(index_data_size * sizeof(unsigned int)),
      m_Material(material)
{
    glCreateVertexArrays(1, &m_VAO);

    unsigned int VBO = 0, EBO = 0;
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferData(VBO, vertex_data_size * sizeof(float), vertex_data,
                      GL_STATIC_DRAW);
    glNamedBufferData(EBO, index_data_size * sizeof(unsigned int), index_data,
                      GL_STATIC_DRAW);

    const int vb_binding_index = 0;
    const int pos_attr = 0;
    glEnableVertexArrayAttrib(m_VAO, pos_attr);
    glVertexArrayAttribBinding(m_VAO, pos_attr, vb_binding_index);
    glVertexArrayAttribFormat(m_VAO, pos_attr, 3, GL_FLOAT, GL_FALSE,
                              sizeof(float) * 0);

    //const int uv_attr = 1;
    //glEnableVertexArrayAttrib(m_VAO, uv_attr);
    //glVertexArrayAttribBinding(m_VAO, uv_attr, vb_binding_index);
    //glVertexArrayAttribFormat(m_VAO, uv_attr, 2, GL_FLOAT, GL_FALSE,
    //                          sizeof(float) * 3);

    glVertexArrayVertexBuffer(m_VAO, vb_binding_index, VBO, 0,
                              sizeof(float) * 5);
    glVertexArrayElementBuffer(m_VAO, EBO);
}

bool Mesh::Primitive::initialize() { return true; }

void Mesh::Primitive::terminate() { glDeleteVertexArrays(1, &m_VAO); }

void Mesh::Primitive::submit(const ShaderProgram& shader_program)
{
    m_Material.submitForRender(shader_program);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_IndexDataSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}  // namespace tamarindo
