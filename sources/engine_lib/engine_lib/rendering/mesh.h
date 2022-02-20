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

#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace tamarindo
{
struct MeshInstanceID {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::size_t VertexCount;
};

class Mesh;

class Vertex
{
   public:
    Vertex(float x, float y, float z, float u, float v);

   private:
    friend class Mesh;

    float m_Positions[3];
    float m_UVs[2];
};

class Mesh
{
   public:
    Mesh() = default;  // TODO: consider delete
    Mesh(unsigned int vertex_size, unsigned int index_size);

    void addVertex(float x, float y, float z, float u, float v);
    void addIndex(unsigned int index);

    [[nodiscard]] static MeshInstanceID createInstance(const Mesh& mesh);
    static void terminateInstance(MeshInstanceID mesh_instance_id);
    static void renderMeshInstance(MeshInstanceID mesh_instance_id);

    unsigned int getVertexDataSize() const;
    const void* getVertexData() const;

    unsigned int getIndexDataSize() const;
    const void* getIndexData() const;

   private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
};
}  // namespace tamarindo
