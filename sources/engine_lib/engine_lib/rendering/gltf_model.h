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

#ifndef ENGINE_LIB_RENDERING_GLTF_MODEL_H_
#define ENGINE_LIB_RENDERING_GLTF_MODEL_H_

#include "rendering/material.h"
#include "rendering/model.h"
#include "world/game_object.h"

#include "tiny_gltf.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace tamarindo
{

struct GLTFPrimitive {
    unsigned int VAO;
    size_t IndexCount;
    int MaterialIndex;
};

struct GLTFMesh {
    std::vector<GLTFPrimitive> Primitives;
};

class GLTFModel : public Model
{
   public:
    GLTFModel(const tinygltf::Model& model);

    bool initialize() override;

    void terminate() override;

    void bindModelNodes(int node_index, tinygltf::Model model,
                        GameObject* parent_game_object);

   private:
    void bindModelNodes(int node_index);
    void bindMesh(int mesh_index);

    tinygltf::Model m_Model;
    std::unordered_map<size_t, unsigned int> m_Buffers;

    // TODO: Change back to private
   public:
    std::vector<Material> m_Materials;

    std::unordered_map<int, GLTFMesh> m_Meshes;

    std::unordered_map<int, std::vector<Transform>> m_MeshInstances;
};

struct GLTFGameObjectDesc {
    std::string ModelPath;
};

class GLTFGameObjectLoader
{
   public:
    static std::unique_ptr<GameObject> load(const GLTFGameObjectDesc& desc);

   private:
    std::unique_ptr<GameObject> loadInternal(const GLTFGameObjectDesc& desc);

    void setMeshFromNode();

    void processModelNode(GameObject* parent_game_object,
                          const tinygltf::Model& model, int node_index);
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_GLTF_MODEL_H_
