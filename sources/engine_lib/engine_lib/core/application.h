// Copyright(c) 2021-2022 Emmanuel Arias
#pragma once
#include "core/window_manager.h"
#include "input/input_manager.h"
#include "logging/logger.h"

#include <chrono>
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

    virtual void doUpdate(std::chrono::duration<double> total_time,
                          std::chrono::duration<double> delta_time) = 0;
    virtual void doRender() = 0;

    virtual void doTerminate() = 0;
    virtual const WindowProperties& getWindowProperties() const = 0;

    Logger m_Logger;

    WindowManager m_WindowManager;
    InputManager m_InputManager;
};

#define g_Application ::tamarindo::Application::get()

}  // namespace tamarindo

extern std::unique_ptr<tamarindo::Application> CreateApplication();
