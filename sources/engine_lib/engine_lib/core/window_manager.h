// Copyright (c) 2021 Emmanuel Arias
#pragma once
#include <array>
#include <memory>
#include <string>

// Forward declarations
struct GLFWwindow;

namespace tamarindo
{
struct WindowProperties {
    std::string Title;
    unsigned int Width;
    unsigned int Height;
    float AspectRatio;
    std::array<float, 4> DefaultBackground = {0.0f, 0.0f, 0.0f, 1.0f};

    WindowProperties() = delete;
    WindowProperties(const std::string& title = "Application",
                     unsigned int width = 800, unsigned int height = 600);
};

class WindowManager
{
   public:
    bool initialize(const WindowProperties& properties);
    void terminate();

    void processEvents();
    void swapBuffers();

    bool shouldWindowClose() const;

   private:
    GLFWwindow* m_Window = nullptr;

    static void glfwErrorCb(int error, const char* desc);
};
}  // namespace tamarindo
