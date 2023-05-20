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

#ifndef ENGINE_LIB_RENDERING_MANAGER_H_
#define ENGINE_LIB_RENDERING_MANAGER_H_

#include "imgui_renderer.h"
#include "scene_renderer.h"

struct GLFWwindow;

namespace tamarindo
{
class Timer;

class RenderingManager
{
   public:
    RenderingManager(GLFWwindow* window);
    ~RenderingManager();

    RenderingManager(const RenderingManager& other) = delete;
    RenderingManager& operator=(const RenderingManager& other) = delete;

    void terminate();

    void update(const Timer& timer);
    void render();

   private:
    std::unique_ptr<ImGuiRenderer> m_ImGuiRenderer;
    std::unique_ptr<SceneRenderer> m_SceneRenderer;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_MANAGER_H_
