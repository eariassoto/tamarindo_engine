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

#ifndef ENGINE_LIB_SCENE_RENDERER_H_
#define ENGINE_LIB_SCENE_RENDERER_H_

#include "engine_lib/rendering/material.h"
#include "engine_lib/rendering/shader_program.h"

#include <memory>

namespace tamarindo
{
class ShaderProgram;
class Timer;

class SceneRenderer
{
   public:
    bool initialize();
    void terminate();
    void render();
    void update(const Timer& timer);

   private:
    bool m_RenderWireframe = true;

    std::unique_ptr<ShaderProgram> m_ShaderProgram = nullptr;

   private:
    Material m_DebugMaterial = Material(tamarindo::Color(53, 99, 124));
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_SCENE_RENDERER_H_
