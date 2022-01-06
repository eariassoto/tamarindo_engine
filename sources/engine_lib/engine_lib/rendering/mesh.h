// Copyright(c) 2021-2022 Emmanuel Arias
#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace tamarindo
{
struct MeshInstanceID {
    unsigned int VertexAttrArray;
    std::size_t IndexSize;
};

struct VertexAttribute {
    unsigned int Size;
    unsigned int Stride;
    unsigned int PosOffset;
};

class Vertex
{
   public:
    Vertex(float x, float y, float z, float u, float v);

    static void defineVertexAttributes();

   private:
    float m_X;
    float m_Y;
    float m_Z;
    float m_U;
    float m_V;

    static constexpr std::array<VertexAttribute, 2> mc_VertexAttributes = {{
        {3, 5 * sizeof(float), 0},                 // Position attribute
        {2, 5 * sizeof(float), 3 * sizeof(float)}  // UV attribute
    }};
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
