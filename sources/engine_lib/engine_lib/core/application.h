// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include "logging/logger.h"

#include <memory>

namespace tamarindo
{
class Application
{
   public:
    virtual ~Application() = default;

    bool initialize();

    void run();

    void terminate();

   private:
    virtual bool doInitialize() = 0;
    virtual void doTerminate() = 0;

    Logger m_Logger;
};

}  // namespace tamarindo

extern std::unique_ptr<tamarindo::Application> CreateApplication();
