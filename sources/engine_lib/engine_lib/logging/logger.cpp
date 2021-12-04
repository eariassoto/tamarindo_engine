// Copyright(c) 2021 Emmanuel Arias
#include "logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace
{
tamarindo::Logger* s_LoggerInstance = nullptr;
}

namespace tamarindo
{
void Logger::initialize()
{
    spdlog::set_pattern("[%T] %^[%l]%$ %n: %v");

    m_SpdLogger = spdlog::stdout_color_mt("TM_CORE");
    m_SpdLogger->set_level(spdlog::level::trace);

    provideInstance(this);
}

void Logger::terminate() { m_SpdLogger.reset(); }

Logger* Logger::getLogger()
{
    // TODO: check if null
    return s_LoggerInstance;
}

void Logger::provideInstance(Logger* instance)
{
    // TODO: check if exists
    if (s_LoggerInstance == nullptr) {
        s_LoggerInstance = instance;
    }
}

}  // namespace tamarindo
