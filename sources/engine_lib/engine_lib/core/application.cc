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
void ApplicationProperties::setWindowTitle(const std::string& window_title)
{
    m_WindowTitle = window_title;
}

void ApplicationProperties::setWindowSize(unsigned int width,
                                          unsigned int height)
{
    if (height == 0) {
        return;
    }

    m_Width = width;
    m_Height = height;
    m_AspectRatio = (float)width / height;
}

void ApplicationProperties::setWindowDefaultBackground(
    const std::array<float, 4>& default_background)
{
    m_DefaultBackground = default_background;
}

bool ApplicationProperties::validate()
{
    bool isValid = true;
    isValid &= !m_WindowTitle.empty();
    isValid &= m_Width > 0;
    isValid &= m_Height > 0;
    return isValid;
}

Application* Application::ptr = nullptr;

bool Application::isRunning() const
{
    return m_IsRunning && !m_WindowManager.shouldWindowClose();
}

void Application::addRenderer(Renderer* renderer_ptr)
{
    m_RenderingManager.addRenderer(renderer_ptr);
}

bool Application::initialize()
{
    // First enable logging for all modules.
    // TODO: Initializing Application and provide API for logger configuration
    m_Logger.initialize();

    if (!doPreInitialize()) {
        TM_LOG_ERROR("Error while pre-initializing the application.");
        return false;
    }

    // TODO: this properties class should be temporary, need better design
    std::unique_ptr<ApplicationProperties> props = loadApplicationProperties();
    if (props == nullptr || !props->validate()) {
        TM_LOG_ERROR("Could not initialize application properties.");
        return false;
    }
    m_Properties = std::move(props);

    TM_LOG_DEBUG("Initializing application");
    m_WindowManager.initialize(*m_Properties.get());
    m_InputManager.initialize();

    m_RenderingManager.initialize();

    if (!doInitialize()) {
        TM_LOG_ERROR("Error while initializing the application.");
        return false;
    }

    ptr = this;
    return true;
}

void Application::run()
{
    const std::array<float, 4>& default_bg =
        m_Properties->WindowDefaultBackground();

    while (isRunning()) {
        m_WindowManager.processEvents();
        m_InputManager.startFrame();

        m_Timer.startFrame();

        // TODO: consider order
        doUpdate(m_Timer);
        m_RenderingManager.updateRenderers(m_Timer);

        glClearColor(default_bg[0], default_bg[1], default_bg[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_RenderingManager.callRenderers();

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

    m_RenderingManager.terminate();

    m_InputManager.terminate();
    m_WindowManager.terminate();

    // Terminate internal modules here
    m_Logger.terminate();
}

}  // namespace tamarindo
