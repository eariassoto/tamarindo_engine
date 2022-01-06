// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include "core/window_manager.h"
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

    void run();

    void terminate();

   private:
    virtual bool doInitialize() = 0;

    virtual void doUpdate(std::chrono::duration<double> total_time,
                          std::chrono::duration<double> delta_time) = 0;
    virtual void doRender() = 0;

    virtual void doTerminate() = 0;
    virtual const WindowProperties& getWindowProperties() const = 0;

    Logger m_Logger;

    WindowManager m_WindowManager;
};

}  // namespace tamarindo

extern std::unique_ptr<tamarindo::Application> CreateApplication();
