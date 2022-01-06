// Copyright (c) 2021 Emmanuel Arias
#include "window_manager.h"

#include "logging/logger.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tamarindo
{
WindowProperties::WindowProperties(const std::string& title, unsigned int width,
                                   unsigned int height)
    : Title{title},
      Width{width},
      Height{height},
      AspectRatio{static_cast<float>(width) / static_cast<float>(height)}
{
}

bool WindowManager::initialize(const WindowProperties& properties)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(properties.Width, properties.Height,
                                properties.Title.c_str(), NULL, NULL);
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

    glViewport(0, 0, properties.Width, properties.Height);

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
