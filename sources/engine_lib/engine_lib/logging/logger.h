// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include "spdlog/spdlog.h"

#include <memory>
#include <string>

namespace tamarindo
{
class Logger
{
   public:
    void initialize();
    void terminate();

    template <typename... Args>
    void trace(fmt::format_string<Args...> fmt, Args &&...args)
    {
        m_SpdLogger->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(fmt::format_string<Args...> fmt, Args &&...args)
    {
        m_SpdLogger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(fmt::format_string<Args...> fmt, Args &&...args)
    {
        m_SpdLogger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args &&...args)
    {
        m_SpdLogger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(fmt::format_string<Args...> fmt, Args &&...args)
    {
        m_SpdLogger->error(fmt, std::forward<Args>(args)...);
    }

    // There will be only one logger instantiated
    static Logger *getLogger();

   private:
    std::shared_ptr<spdlog::logger> m_SpdLogger = nullptr;

    static void provideInstance(Logger *instance);
};
}  // namespace tamarindo

// Macros for easy logging
#define TM_LOG_TRACE(...) ::tamarindo::Logger::getLogger()->trace(__VA_ARGS__)
#define TM_LOG_INFO(...) ::tamarindo::Logger::getLogger()->info(__VA_ARGS__)
#define TM_LOG_DEBUG(...) ::tamarindo::Logger::getLogger()->debug(__VA_ARGS__)
#define TM_LOG_WARN(...) ::tamarindo::Logger::getLogger()->warn(__VA_ARGS__)
#define TM_LOG_ERROR(...) ::tamarindo::Logger::getLogger()->error(__VA_ARGS__)