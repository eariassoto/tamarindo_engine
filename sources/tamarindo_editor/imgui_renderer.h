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

#ifndef TAMARINDO_EDITOR_IMGUI_RENDERER_H_
#define TAMARINDO_EDITOR_IMGUI_RENDERER_H_

#include "engine_lib/rendering/renderer.h"

namespace tamarindo
{
class GameObject2;
}
class Scene;

class ImGuiRenderer : public tamarindo::Renderer
{
   public:
    ImGuiRenderer(Scene* scene_ptr);

    bool initialize() override;
    void terminate() override;
    void render() override;
    void update(const tamarindo::Timer& timer) override;

   private:
    void setupColorStyle();

    void renderSceneTree(const tamarindo::GameObject2& curr_node);

    Scene* m_ScenePtr = nullptr;
};

#endif  // TAMARINDO_EDITOR_IMGUI_RENDERER_H_
