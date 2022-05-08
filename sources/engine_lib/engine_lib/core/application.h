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

#ifndef ENGINE_LIB_CORE_APPLICATION_H_
#define ENGINE_LIB_CORE_APPLICATION_H_

#include "core/timer.h"
#include "core/window_manager.h"
#include "input/input_manager.h"
#include "logging/logger.h"
#include "rendering/renderer.h"
#include "world/scene.h"

#include <memory>

namespace tamarindo
{
// TODO: Refactor this, make this only for init and save information in modules
class ApplicationProperties
{
   public:
    bool validate();

    void setWindowTitle(const std::string& window_title);
    void setWindowSize(unsigned int width, unsigned int height);
    void setWindowDefaultBackground(
        const std::array<float, 4>& default_background);

    inline const std::string& WindowTitle() const { return m_WindowTitle; }
    inline unsigned int WindowWidth() const { return m_Width; }
    inline unsigned int WindowHeight() const { return m_Height; }
    inline const std::array<float, 4>& WindowDefaultBackground() const
    {
        return m_DefaultBackground;
    }

   private:
    std::string m_WindowTitle;
    unsigned int m_Width;
    unsigned int m_Height;
    float m_AspectRatio;
    std::array<float, 4> m_DefaultBackground = {0.0f, 0.0f, 0.0f, 1.0f};
};

class Application
{
   public:
    virtual ~Application() = default;

    bool initialize();
    void terminate();

    void run();
    void stop();

    void loadScene(std::unique_ptr<Scene> new_scene);

    static Application* get();

   private:
    bool m_IsRunning = true;

    bool isRunning() const;

    virtual bool doInitialize() = 0;

    virtual void doUpdate(const Timer& timer) = 0;

    virtual bool doPreInitialize() = 0;
    virtual std::unique_ptr<ApplicationProperties>
    loadApplicationProperties() = 0;
    virtual void doTerminate() = 0;

    // This pointer will be checked so there should not be UB
    inline const ApplicationProperties& getProperties() const
    {
        return *m_Properties.get();
    }

    // imgui UI
    void initializeImguiUI();
    virtual void setupColorStyleImguiUI();
    void terminateImguiUI();
    void renderImguiUI();

   private:
    std::unique_ptr<ApplicationProperties> m_Properties = nullptr;

    std::unique_ptr<Scene> m_CurrentScene = nullptr;

    Timer m_Timer;
    Logger m_Logger;

    WindowManager m_WindowManager;
    InputManager m_InputManager;

    Renderer m_Renderer;
};

#define g_Application ::tamarindo::Application::get()

}  // namespace tamarindo

extern std::unique_ptr<tamarindo::Application> CreateApplication();

#endif  // ENGINE_LIB_CORE_APPLICATION_H_
