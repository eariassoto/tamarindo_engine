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

#ifndef ENGINE_LIB_CORE_APPLICATION_H_
#define ENGINE_LIB_CORE_APPLICATION_H_

#include "engine_lib/core/timer.h"
#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/imgui_renderer.h"
#include "engine_lib/rendering/scene_renderer.h"
#include "engine_lib/world/scene.h"

#include <memory>

namespace tamarindo
{
class Renderer;
class Scene;

class Application
{
   public:
    Application() = default;
    virtual ~Application() = default;

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    bool initialize();
    void terminate();

    void run();
    void stop();

    Scene* getMainScene() const;

    static Application* ptr;

   private:
    bool m_IsRunning = true;

    bool isRunning() const;

    virtual bool doInitialize() = 0;

    virtual void doUpdate(const Timer& timer) = 0;

    virtual bool doPreInitialize() = 0;
    virtual void doTerminate() = 0;

   protected:
    std::unique_ptr<Scene> m_MainScene = nullptr;

   private:
    Timer m_Timer;
    Logger m_Logger;

    GLFWwindow* m_Window = nullptr;

    std::unique_ptr<InputManager> m_InputManager;

    std::unique_ptr<ImGuiRenderer> m_ImGuiRenderer;
    std::unique_ptr<SceneRenderer> m_SceneRenderer;
};

#define g_Application ::tamarindo::Application::ptr

}  // namespace tamarindo

#endif  // ENGINE_LIB_CORE_APPLICATION_H_
