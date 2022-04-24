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

#include <glad/glad.h>

namespace tamarindo
{
Mesh::Mesh(unsigned int primitive_count)
{
    m_Primitives.reserve(primitive_count);
}

void Mesh::addPrimitive(std::vector<float> vertices,
                        std::vector<unsigned int> indices)
{
    m_Primitives.emplace_back(vertices, indices);
}

bool Mesh::initialize() { 
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

void Mesh::submit() {
    for (Primitive& p : m_Primitives) {
        p.submit();
    }
}

Mesh::Primitive::Primitive(std::vector<float> vertices,
                           std::vector<unsigned int> indices)
    : m_Vertices(vertices), m_Indices(indices)
{
}

bool Mesh::Primitive::initialize()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // TODO: Create enum to speficy vertex format
    glVertexAttribFormat(0, 3, GL_FLOAT, false, sizeof(float) * 0);
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribFormat(1, 3, GL_FLOAT, false, sizeof(float) * 3);
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float),
                 m_Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m_Indices.size() * sizeof(unsigned int), m_Indices.data(),
                 GL_STATIC_DRAW);

    return true;
}

void Mesh::Primitive::terminate() { glDeleteVertexArrays(1, &m_VAO); }

void Mesh::Primitive::submit()
{
    glBindVertexArray(m_VAO);

    glBindVertexBuffer(0, m_VBO, 0, sizeof(float) * 5);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    glDrawElements(GL_TRIANGLES,
                   (GLsizei)m_Indices.size() * sizeof(unsigned int),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}  // namespace tamarindo
