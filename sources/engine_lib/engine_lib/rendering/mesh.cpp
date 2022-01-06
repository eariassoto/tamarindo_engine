// Copyright(c) 2021-2022 Emmanuel Arias
#include "mesh.h"

#include <glad/glad.h>

namespace tamarindo
{
Vertex::Vertex(float x, float y, float z, float u, float v)
    : m_X(x), m_Y(y), m_Z(z), m_U(u), m_V(v)
{
}

/*static*/ void Vertex::defineVertexAttributes()
{
    for (int i = 0; i < mc_VertexAttributes.size(); ++i) {
        const VertexAttribute& attr = mc_VertexAttributes[i];

        glVertexAttribPointer(i, attr.Size, GL_FLOAT, GL_FALSE, attr.Stride,
                              (void*)attr.PosOffset);
        glEnableVertexAttribArray(i);
    }
}

Mesh::Mesh(unsigned int vertex_size, unsigned int index_size)
{
    m_Vertices.reserve(vertex_size);
    m_Indices.reserve(index_size);
}

void Mesh::addVertex(float x, float y, float z, float u, float v)
{
    m_Vertices.emplace_back(x, y, z, u, v);
}

void Mesh::addIndex(unsigned int index) { m_Indices.emplace_back(index); }

/*static*/ MeshInstanceID Mesh::createInstance(const Mesh& mesh)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataSize(),
                 mesh.getVertexData(), GL_STATIC_DRAW);

    unsigned int EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexDataSize(),
                 mesh.getIndexData(), GL_STATIC_DRAW);

    Vertex::defineVertexAttributes();

    return {VAO, mesh.getIndexDataSize()};
}

/*static*/ void Mesh::terminateInstance(MeshInstanceID mesh_instance_id)
{
    glDeleteVertexArrays(1, &mesh_instance_id.VertexAttrArray);
}

/*static*/ void Mesh::renderMeshInstance(MeshInstanceID mesh_instance_id)
{
    glBindVertexArray(mesh_instance_id.VertexAttrArray);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh_instance_id.IndexSize, GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

unsigned int Mesh::getVertexDataSize() const
{
    return m_Vertices.size() * sizeof(Vertex);
}

const void* Mesh::getVertexData() const
{
    return static_cast<const void*>(m_Vertices.data());
}

unsigned int Mesh::getIndexDataSize() const
{
    return m_Indices.size() * sizeof(unsigned int);
}

const void* Mesh::getIndexData() const
{
    return static_cast<const void*>(m_Indices.data());
}

}  // namespace tamarindo