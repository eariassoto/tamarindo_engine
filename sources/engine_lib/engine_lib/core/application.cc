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

#include "application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tamarindo
{
namespace
{
Application* s_Application = nullptr;
}

Application* Application::get() { return s_Application; }

bool Application::isRunning() const
{
    return m_IsRunning && !m_WindowManager.shouldWindowClose();
}

bool Application::initialize()
{
    s_Application = this;

    // Initialize internal modules here
    m_Logger.initialize();
    TM_LOG_DEBUG("Initializing application");

    m_WindowManager.initialize(getWindowProperties());
    m_InputManager.initialize();

    return doInitialize();
}

void Application::run()
{
    const std::array<float, 4>& default_bg =
        getWindowProperties().DefaultBackground;

    while (isRunning()) {
        m_WindowManager.processEvents();
        m_InputManager.startFrame();

        m_Timer.startFrame();

        if ((m_Timer.frameCount() % 200) == 0) {
            TM_LOG_TRACE("last frame time {}s", 1.0/m_Timer.deltaTime());
        }

        doUpdate(m_Timer);

        glClearColor(default_bg[0], default_bg[1], default_bg[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        doRender();

        m_InputManager.finishFrame();

        m_WindowManager.swapBuffers();

        m_Timer.endFrame();
    }
}

void Application::stop() { m_IsRunning = false; }

void Application::terminate()
{
    TM_LOG_DEBUG("Terminating application");
    doTerminate();

    m_InputManager.terminate();
    m_WindowManager.terminate();

    // Terminate internal modules here
    m_Logger.terminate();
}

}  // namespace tamarindo
