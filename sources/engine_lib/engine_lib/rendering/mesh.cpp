// Copyright(c) 2021 Emmanuel Arias
#include "mesh.h"

#include <glad/glad.h>

namespace tamarindo
{
Mesh::Mesh(unsigned int vertex_size, unsigned int index_size)
{
    Vertices.reserve(vertex_size);
    Indices.reserve(index_size);
}

void Mesh::addVertex(float x, float y, float z, float u, float v)
{
    Vertices.push_back(x);
    Vertices.push_back(y);
    Vertices.push_back(z);
    Vertices.push_back(u);
    Vertices.push_back(v);
}

void Mesh::addIndex(unsigned int index) { Indices.push_back(index); }

/*static*/ MeshInstanceID Mesh::createInstance(const Mesh& mesh)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(float),
                 mesh.Vertices.data(), GL_STATIC_DRAW);

    unsigned int EBO = 0;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.Indices.size() * sizeof(unsigned int),
                 mesh.Indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, VERTEX_ATTR_POS_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_ATTR_STRIDE, (void*)VERTEX_ATTR_POS_OFFSET);
    glEnableVertexAttribArray(0);
    // UVs attribute
    glVertexAttribPointer(1, VERTEX_ATTR_UV_SIZE, GL_FLOAT, GL_FALSE,
                          VERTEX_ATTR_STRIDE, (void*)VERTEX_ATTR_UV_OFFSET);
    glEnableVertexAttribArray(1);

    return {VAO, mesh.Indices.size()};
}

/*static*/ void Mesh::terminateInstance(MeshInstanceID mesh_instance_id)
{
    glDeleteVertexArrays(1, &mesh_instance_id.VertexAttrArray);
}

/*static*/ void Mesh::renderMeshInstance(MeshInstanceID mesh_instance_id)
{
    glBindVertexArray(mesh_instance_id.VertexAttrArray);
    glDrawElements(GL_TRIANGLES, mesh_instance_id.IndexSize, GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

}  // namespace tamarindo