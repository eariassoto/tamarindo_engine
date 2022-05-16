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

#ifndef ENGINE_LIB_RENDERING_MESH_H_
#define ENGINE_LIB_RENDERING_MESH_H_

#include "rendering/material.h"
#include "rendering/mesh_interface.h"

#include <vector>

namespace tamarindo
{
class ShaderProgram;

class Mesh : public MeshInterface
{
   public:
    Mesh(unsigned int primitive_count);

    void addPrimitive(const float* vertex_data, unsigned int vertex_data_size,
                      const unsigned int* index_data,
                      unsigned int index_data_size, const Material& material);
    bool initialize() override;

    void terminate() override;
    void submit(const ShaderProgram& shader_program) override;

   private:
    class Primitive
    {
       public:
        Primitive(const float* vertex_data, unsigned int vertex_data_size,
                  const unsigned int* index_data, unsigned int index_data_size,
                  const Material& material);

        bool initialize();

        void terminate();
        void submit(const ShaderProgram& shader_program);

       private:
        unsigned int m_IndexDataSize = 0;
        unsigned int m_VAO = 0;

        Material m_Material;
    };

    std::vector<Primitive> m_Primitives;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_MESH_H_
