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
#ifndef ENGINE_LIB_RESOURCES_MANAGER_H_
#define ENGINE_LIB_RESOURCES_MANAGER_H_

#include "engine_lib/logging/logger.h"

struct BufferDesc {
    const void* data;
    long int size;
};

struct VertexArrayAtrributeDesc {
    unsigned int location;
    unsigned int bindingIndex;

    int size;
    int componentType;

    bool isNormalized;

    unsigned int byteOffset;

    int stride;

    unsigned int buffer;
};

struct VertexArrayDesc {
    unsigned int elementArrayBuffer;

    std::vector<VertexArrayAtrributeDesc> atributeData;
};

class ResourcesManager
{
   public:
    static bool createBuffer(const BufferDesc& desc, unsigned int* buffer);

    static bool createVertexArray(const VertexArrayDesc& desc,
                                  unsigned int* vertex_array);

    static void releaseBuffer(unsigned int buffer);

    static void releaseVertexArray(unsigned int vertex_array);
};

#endif  // ENGINE_LIB_RESOURCES_MANAGER_H_