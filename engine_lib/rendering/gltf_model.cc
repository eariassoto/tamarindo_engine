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

// define only once
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "engine_lib/rendering/gltf_model.h"

#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/resources_manager.h"
#include "engine_lib/rendering/shader_program.h"

#include <cassert>

namespace tamarindo
{
GLTFModel::GLTFModel(const tinygltf::Model& model) : m_Model(model) {}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

void GLTFModel::bindMesh(int mesh_index)
{
    if (m_Meshes.find(mesh_index) != m_Meshes.end()) {
        return;
    }

    const tinygltf::Mesh& mesh = m_Model.meshes[mesh_index];
    TM_LOG_INFO("Processing mesh {}: {}", mesh_index, mesh.name);
    GLTFMesh gltf_mesh;

    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive primitive = mesh.primitives[i];

        VertexArrayDesc desc;

        for (auto& [name, buffer_view_index] : primitive.attributes) {
            tinygltf::Accessor accessor = m_Model.accessors[buffer_view_index];

            const tinygltf::BufferView& buffer_view =
                m_Model.bufferViews[accessor.bufferView];
            int byte_stride = accessor.ByteStride(buffer_view);
            int size = accessor.type == TINYGLTF_TYPE_SCALAR
                           ? 1
                           : size = accessor.type;

            int attr_location = -1;
            const int vb_binding_index = 0;
            if (name.compare("POSITION") == 0) {
                attr_location = 0;
            } else if (name.compare("TEXCOORD_0") == 0) {
                attr_location = -1;
            }

            if (attr_location != -1) {
                TM_LOG_INFO("For attribute {}, location = {}", name,
                            attr_location);

                VertexArrayAtrributeDesc attr_desc;

                attr_desc.location = attr_location;
                attr_desc.bindingIndex = vb_binding_index;
                attr_desc.size = size;
                attr_desc.componentType = accessor.componentType;
                attr_desc.isNormalized = accessor.normalized;
                attr_desc.byteOffset = (unsigned int)accessor.byteOffset;
                attr_desc.stride = byte_stride;
                attr_desc.buffer = m_Buffers[buffer_view_index];

                desc.atributeData.push_back(attr_desc);
            }
        }

        tinygltf::Accessor index_accessor =
            m_Model.accessors[primitive.indices];

        desc.elementArrayBuffer = m_Buffers[index_accessor.bufferView];

        unsigned int vao;
        ResourcesManager::createVertexArray(desc, &vao);

        gltf_mesh.Primitives.push_back(
            GLTFPrimitive{vao, index_accessor.count, primitive.material});
    }
    m_Meshes[mesh_index] = gltf_mesh;
}

void GLTFModel::bindModelNodes(int node_index)
{
    assert((node_index >= 0) && (node_index < m_Model.nodes.size()));
    const tinygltf::Node& node = m_Model.nodes[node_index];
    TM_LOG_INFO("Processing node {}: {}", node_index, node.name);
    if ((node.mesh >= 0) && (node.mesh < m_Model.meshes.size())) {
        Transform t;
        if (node.translation.size() == 3) {
            t.setPosition(glm::vec3((float)node.translation.at(0),
                                    (float)node.translation.at(1),
                                    (float)node.translation.at(2)));
        }
        if (node.scale.size() == 3) {
            t.setScale(glm::vec3((float)node.scale.at(0),
                                 (float)node.scale.at(1),
                                 (float)node.scale.at(2)));
        }
        if (node.rotation.size() == 4) {
            t.setRotation(glm::quat(
                (float)node.rotation.at(3), (float)node.rotation.at(0),
                (float)node.rotation.at(1), (float)node.rotation.at(2)));
        }
        bindMesh(node.mesh);
        m_MeshInstances[node.mesh].push_back(t);
    }

    for (const int child_node_index : node.children) {
        bindModelNodes(child_node_index);
    }
}

bool GLTFModel::initialize()
{
    for (size_t i = 0; i < m_Model.bufferViews.size(); ++i) {
        const tinygltf::BufferView& buffer_view = m_Model.bufferViews[i];

        if (buffer_view.target != TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER &&
            buffer_view.target != TINYGLTF_TARGET_ARRAY_BUFFER) {
            continue;
        }

        const tinygltf::Buffer& buffer = m_Model.buffers[buffer_view.buffer];

        BufferDesc desc;
        desc.data = &buffer.data.at(0) + buffer_view.byteOffset;
        desc.size = (long)buffer_view.byteLength;

        unsigned int vbo = 0;
        ResourcesManager::createBuffer(desc, &vbo);
        m_Buffers[i] = vbo;

        TM_LOG_INFO(
            "Buffer view: {}, buffer.data.size = {}, "
            "bufferview.byteOffset = {}",
            i, buffer.data.size(), buffer_view.byteOffset);
    }

    for (const tinygltf::Material& mat : m_Model.materials) {
        auto& base_color = mat.pbrMetallicRoughness.baseColorFactor;
        m_Materials.emplace_back(Material(Color(
            (float)base_color[0], (float)base_color[1], (float)base_color[2])));
    }

    const tinygltf::Scene& scene = m_Model.scenes[m_Model.defaultScene];
    for (const int node_index : scene.nodes) {
        bindModelNodes(node_index);
    }

    return true;
}

void GLTFModel::terminate()
{
    for (auto [key, value] : m_Buffers) {
        ResourcesManager::releaseBuffer(value);
    }
    for (auto [key, value] : m_Meshes) {
        for (const auto& p : value.Primitives) {
            ResourcesManager::releaseVertexArray(p.VAO);
        }
    }
}

/*static*/ std::unique_ptr<GameObject> GLTFGameObjectLoader::load(
    const GLTFGameObjectDesc& desc)
{
    GLTFGameObjectLoader l;
    return l.loadInternal(desc);
}

void setTransformFromNode(Transform* t, const tinygltf::Node& node)
{
    if (node.translation.size() == 3) {
        t->setPosition(glm::vec3((float)node.translation.at(0),
                                 (float)node.translation.at(1),
                                 (float)node.translation.at(2)));
    }
    if (node.scale.size() == 3) {
        t->setScale(glm::vec3((float)node.scale.at(0), (float)node.scale.at(1),
                              (float)node.scale.at(2)));
    }
    if (node.rotation.size() == 4) {
        t->setRotation(
            glm::quat((float)node.rotation.at(3), (float)node.rotation.at(0),
                      (float)node.rotation.at(1), (float)node.rotation.at(2)));
    }
}

void GLTFGameObjectLoader::setMeshFromNode() {}

void GLTFGameObjectLoader::processModelNode(GameObject* parent_game_object,
                                            const tinygltf::Model& model,
                                            int node_index)
{
    assert((node_index >= 0) && (node_index < model.nodes.size()));
    const tinygltf::Node& node = model.nodes[node_index];
    TM_LOG_INFO("Processing node {}: {}", node_index, node.name);

    GameObject* game_object =
        (node.name.empty()) ? new GameObject() : new GameObject(node.name);

    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        TM_LOG_INFO("Node has mesh");
        setTransformFromNode(&game_object->m_Transform, node);
        setMeshFromNode();
    }
    /*if ((node.mesh >= 0) && (node.mesh < m_Model.meshes.size())) {
        Transform t;

        bindMesh(node.mesh);
        m_MeshInstances[node.mesh].push_back(t);
    }*/

    parent_game_object->addChild(game_object);

    for (const int child_node_index : node.children) {
        processModelNode(game_object, model, child_node_index);
    }
}

std::unique_ptr<GameObject> GLTFGameObjectLoader::loadInternal(
    const GLTFGameObjectDesc& desc)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool loaded =
        loader.LoadBinaryFromFile(&model, &err, &warn, desc.ModelPath);
    if (!loaded) {
        return nullptr;
    }

    std::unique_ptr<GameObject> root_go = std::make_unique<GameObject>();

    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (const int node_index : scene.nodes) {
        processModelNode(root_go.get(), model, node_index);
    }

    return root_go;
}

}  // namespace tamarindo
