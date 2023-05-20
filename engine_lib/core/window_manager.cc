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

#include "engine_lib/core/window_manager.h"

#include "engine_lib/core/application.h"
#include "engine_lib/logging/logger.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tamarindo
{

bool WindowManager::initialize(const ApplicationProperties& properties)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    
    m_Window =
        glfwCreateWindow(properties.WindowWidth(), properties.WindowHeight(),
                         properties.WindowTitle().c_str(), NULL, NULL);

    if (m_Window == NULL) {
        TM_LOG_ERROR("Failed to create GLFW window");
        return false;
    }
    
    glfwSetErrorCallback(WindowManager::glfwErrorCb);

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        TM_LOG_ERROR("Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, properties.WindowWidth(), properties.WindowHeight());

    glEnable(GL_DEPTH_TEST);

    TM_LOG_TRACE("Window Manager initialized");

    return true;
}

void WindowManager::terminate() { glfwTerminate(); }

void WindowManager::processEvents() { glfwPollEvents(); }

void WindowManager::swapBuffers() { glfwSwapBuffers(m_Window); }

bool WindowManager::shouldWindowClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void WindowManager::glfwErrorCb(int error, const char* desc)
{
    TM_LOG_ERROR("GLFW error {}: {}", error, desc);
}

}  // namespace tamarindo
