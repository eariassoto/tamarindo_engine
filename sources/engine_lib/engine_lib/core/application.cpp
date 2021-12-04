// Copyright(c) 2021 Emmanuel Arias
#include "application.h"

namespace tamarindo
{
bool Application::initialize()
{
    // Initialize internal modules here
    m_Logger.initialize();
    TM_LOG_DEBUG("Initializing application");

    return doInitialize();
}

void Application::run() { TM_LOG_DEBUG("Running application"); }

void Application::terminate()
{
    TM_LOG_DEBUG("Terminating application");
    doTerminate();

    // Terminate internal modules here
    m_Logger.terminate();
}

}  // namespace tamarindo
