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

#include "engine_lib/core/application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tamarindo
{

namespace
{
void glfwErrorCb(int error, const char* desc)
{
    TM_LOG_ERROR("GLFW error {}: {}", error, desc);
}

GLFWwindow* initializeGlfwWithWindow(int width, int height,
                                     const std::string& title)
{
    if (GLFW_FALSE == glfwInit()) {
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =
        glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (window == NULL) {
        TM_LOG_ERROR("Failed to create GLFW window");
        return nullptr;
    }

    glfwSetErrorCallback(glfwErrorCb);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        TM_LOG_ERROR("Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    TM_LOG_TRACE("Window Manager initialized");

    return window;
}

}  // namespace

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
    return m_IsRunning && !glfwWindowShouldClose(m_Window);
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

    m_Window = initializeGlfwWithWindow(m_Properties->WindowWidth(),
                                        m_Properties->WindowHeight(),
                                        m_Properties->WindowTitle());

    m_InputManager = std::make_unique<InputManager>(m_Window);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                    int action, int mods) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->m_InputManager->keyCallback(key, scancode, action, mods);
    });

    m_SceneRenderer = std::make_unique<SceneRenderer>(m_Window);
    m_ImGuiRenderer = std::make_unique<ImGuiRenderer>(m_Window);

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
        glfwPollEvents();
        m_InputManager->startFrame();

        m_Timer.startFrame();

        // TODO: consider order
        doUpdate(m_Timer);
        m_SceneRenderer->update(m_Timer);
        m_ImGuiRenderer->update(m_Timer);

        glClearColor(default_bg[0], default_bg[1], default_bg[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_SceneRenderer->render();
        m_ImGuiRenderer->render();

        m_InputManager->finishFrame();

        glfwSwapBuffers(m_Window);

        m_Timer.endFrame();
    }
}

void Application::stop() { m_IsRunning = false; }

Scene* Application::getMainScene() const { return m_MainScene.get(); }

void Application::terminate()
{
    TM_LOG_DEBUG("Terminating application");
    doTerminate();

    m_ImGuiRenderer->terminate();
    m_SceneRenderer->terminate();

    glfwTerminate();

    // Terminate internal modules here
    m_Logger.terminate();
}

}  // namespace tamarindo
