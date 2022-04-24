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

#pragma once
#include "core/timer.h"
#include "core/window_manager.h"
#include "input/input_manager.h"
#include "logging/logger.h"

#include <memory>

namespace tamarindo
{
struct WindowProperties;

class Application
{
   public:
    virtual ~Application() = default;

    bool initialize();
    void terminate();

    void run();
    void stop();

    static Application* get();

   private:
    bool m_IsRunning = true;

    bool isRunning() const;

    virtual bool doInitialize() = 0;

    virtual void doUpdate(const Timer& timer) = 0;
    virtual void doRender() = 0;

    virtual void doTerminate() = 0;
    virtual const WindowProperties& getWindowProperties() const = 0;

    Timer m_Timer;

    Logger m_Logger;

    WindowManager m_WindowManager;
    InputManager m_InputManager;
};

#define g_Application ::tamarindo::Application::get()

}  // namespace tamarindo

extern std::unique_ptr<tamarindo::Application> CreateApplication();
