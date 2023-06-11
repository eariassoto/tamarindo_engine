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
#include "engine_lib/rendering/resources_manager.h"

#include "glad/glad.h"

void CheckOpenGLErr()
{
    const GLenum err = glGetError();
    if (GL_NO_ERROR != err) {
        TM_LOG_ERROR("GL Error: {}", err);
    }
}

bool ResourcesManager::createBuffer(const BufferDesc& desc,
                                    unsigned int* buffer)
{
    GLuint buff = -1;
    glCreateBuffers(1, &buff);
    CheckOpenGLErr();

    glNamedBufferData(buff, desc.size, desc.data, GL_STATIC_DRAW);

    *buffer = buff;

    return true;
}

void ResourcesManager::releaseBuffer(unsigned int buffer)
{
    glDeleteBuffers(1, &buffer);
}

bool ResourcesManager::createVertexArray(const VertexArrayDesc& desc,
                                         unsigned int* vertex_array)
{
    GLuint vao = -1;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for (const VertexArrayAtrributeDesc& attr_desc : desc.atributeData) {
        glEnableVertexArrayAttrib(vao, attr_desc.location);
        glVertexArrayAttribBinding(vao, attr_desc.location,
                                   attr_desc.bindingIndex);
        glVertexArrayAttribFormat(vao, attr_desc.location, attr_desc.size,
                                  attr_desc.componentType,
                                  attr_desc.isNormalized, attr_desc.byteOffset);

        glVertexArrayVertexBuffer(vao, attr_desc.location, attr_desc.buffer,
                                  attr_desc.byteOffset, attr_desc.stride);
    }

    glVertexArrayElementBuffer(vao, desc.elementArrayBuffer);

    *vertex_array = vao;
    glBindVertexArray(0);
    return true;
}

void ResourcesManager::releaseVertexArray(unsigned int vertex_array)
{
    glDeleteVertexArrays(1, &vertex_array);
}
