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

#ifndef TAMARINDO_EDITOR_SCENE_RENDERER_H_
#define TAMARINDO_EDITOR_SCENE_RENDERER_H_

#include "engine_lib/rendering/renderer.h"
#include "engine_lib/rendering/shader_program.h"

#include <memory>

class Scene;

namespace tamarindo
{
class ShaderProgram;
}  // namespace tamarindo

class SceneRenderer : public tamarindo::Renderer
{
   public:
    SceneRenderer(Scene* scene_ptr);

    bool initialize() override;
    void terminate() override;
    void render() override;

   private:
    Scene* m_ScenePtr;
    std::unique_ptr<tamarindo::ShaderProgram> m_ShaderProgram = nullptr;
};

#endif  // TAMARINDO_EDITOR_SCENE_RENDERER_H_
