// Copyright (c) 2021-2022 Emmanuel Arias
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
    WindowProperties(const std::string& title, unsigned int width,
                     unsigned int height);
};

class WindowManager
{
   public:
    bool initialize(const WindowProperties& properties);
    void terminate();

    void processEvents();
    void swapBuffers();

    bool shouldWindowClose() const;

    static GLFWwindow* get();

   private:
    GLFWwindow* m_Window = nullptr;

    static void glfwErrorCb(int error, const char* desc);
};

#define g_Window ::tamarindo::WindowManager::get()

}  // namespace tamarindo
