// Copyright(c) 2021-2022 Emmanuel Arias
#include "mesh.h"

#include <glad/glad.h>

namespace tamarindo
{
Vertex::Vertex(float x, float y, float z, float u, float v)
    : m_Positions{x, y, z}, m_UVs{u, v}
{
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
    MeshInstanceID new_instance;

    glGenVertexArrays(1, &new_instance.VAO);
    glBindVertexArray(new_instance.VAO);

    glVertexAttribFormat(0, 3, GL_FLOAT, false, offsetof(Vertex, m_Positions));
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribFormat(1, 3, GL_FLOAT, false, offsetof(Vertex, m_UVs));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &new_instance.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, new_instance.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertexDataSize(),
                 mesh.getVertexData(), GL_STATIC_DRAW);

    glGenBuffers(1, &new_instance.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_instance.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexDataSize(),
                 mesh.getIndexData(), GL_STATIC_DRAW);

    new_instance.VertexCount = mesh.getIndexDataSize();

    return new_instance;
}

/*static*/ void Mesh::terminateInstance(MeshInstanceID mesh_instance_id)
{
    glDeleteVertexArrays(1, &mesh_instance_id.VAO);
}

/*static*/ void Mesh::renderMeshInstance(MeshInstanceID mesh_instance_id)
{
    glBindVertexArray(mesh_instance_id.VAO);

    glBindVertexBuffer(0, mesh_instance_id.VBO, 0, sizeof(Vertex));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_instance_id.EBO);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh_instance_id.VertexCount,
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int Mesh::getVertexDataSize() const
{
    return static_cast<unsigned int>(m_Vertices.size() * sizeof(Vertex));
}

const void* Mesh::getVertexData() const
{
    return static_cast<const void*>(m_Vertices.data());
}

unsigned int Mesh::getIndexDataSize() const
{
    return static_cast<unsigned int>(m_Indices.size() * sizeof(unsigned int));
}

const void* Mesh::getIndexData() const
{
    return static_cast<const void*>(m_Indices.data());
}

}  // namespace tamarindo