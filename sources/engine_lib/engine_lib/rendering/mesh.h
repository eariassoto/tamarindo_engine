// Copyright(c) 2021-2022 Emmanuel Arias
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
