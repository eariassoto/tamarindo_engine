// Copyright(c) 2021 Emmanuel Arias
#pragma once

#include <vector>

namespace tamarindo
{
struct MeshInstanceID {
    unsigned int VertexAttrArray;
    unsigned int IndexSize;
};

struct Mesh
{
   public:
    Mesh() = default;  // TODO: consider delete
    Mesh(unsigned int vertex_size, unsigned int index_size);

    void addVertex(float x, float y, float z, float u, float v);
    void addIndex(unsigned int index);

    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;

    [[nodiscard]] static MeshInstanceID createInstance(const Mesh& mesh);
    static void terminateInstance(MeshInstanceID mesh_instance_id);
    static void renderMeshInstance(MeshInstanceID mesh_instance_id);

    // TODO: Make this flexible
   private:
    static constexpr int VERTEX_ATTR_POS_SIZE = 3;
    static constexpr int VERTEX_ATTR_UV_SIZE = 2;

    static constexpr int VERTEX_ATTR_STRIDE = 5 * sizeof(float);

    static constexpr int VERTEX_ATTR_POS_OFFSET = 0;
    static constexpr int VERTEX_ATTR_UV_OFFSET = 3 * sizeof(float);
};
}  // namespace tamarindo
