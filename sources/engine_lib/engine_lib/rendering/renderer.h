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

#pragma once

#include "rendering/material.h"
#include "rendering/mesh.h"
#include "rendering/rendering_asset.h"
#include "rendering/shader_program.h"

#include <memory>
#include <unordered_map>

namespace tamarindo
{
class Scene;

class Renderer
{
   public:
    bool initialize();
    void terminate();

    void submitScene(const Scene& scene);

    void registerResources(const std::vector<RenderingAsset*> assets);

   private:
    std::unordered_map<unsigned int, Material*> m_Materials;
    std::unordered_map<unsigned int, Mesh*> m_Meshes;

    std::unique_ptr<ShaderProgram> m_ShaderProgram = nullptr;
};
}  // namespace tamarindo
