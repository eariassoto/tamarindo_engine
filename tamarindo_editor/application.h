/*
 Copyright 2021-2023 Emmanuel Arias Soto

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

#ifndef TAMARINDO_EDITOR_APPLICATION_H_
#define TAMARINDO_EDITOR_APPLICATION_H_

#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/imgui_renderer.h"
#include "engine_lib/rendering/scene_renderer.h"
#include "engine_lib/utils/timer.h"
#include "engine_lib/rendering/scene.h"

#include <memory>

namespace tamarindo
{
class Renderer;
class Scene;

class Application
{
   public:
    Application() = default;
    ~Application() = default;

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    bool initialize();
    void terminate();

    void run();
    void stop();

    Scene* getMainScene() const;

   private:
    bool m_IsRunning = true;

    bool isRunning() const;

   protected:
    std::unique_ptr<Scene> m_MainScene = nullptr;

   private:
    Logger m_Logger;

    GLFWwindow* m_Window = nullptr;

    std::unique_ptr<InputManager> m_InputManager;

    std::unique_ptr<ImGuiRenderer> m_ImGuiRenderer;
    std::unique_ptr<SceneRenderer> m_SceneRenderer;
};

}  // namespace tamarindo

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
