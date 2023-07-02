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

#ifndef ENGINE_LIB_LOGGING_LOGGER_H_
#define ENGINE_LIB_LOGGING_LOGGER_H_

#define FMT_HEADER_ONLY
#include "spdlog/spdlog.h"

#include "fmt/core.h"

#include <memory>

namespace tamarindo::logging
{

// This logger provides a simple spdlog instance, saved as a global pointer. The
// logging macros will use this logger to print messages if an instance is
// alive, otherwise it will use fmt as fallback. This fallback logic should only
// be used for testing purposes.
struct ScopedSpdLogger {
    ScopedSpdLogger();
    ~ScopedSpdLogger();

    static spdlog::logger *Get();
};

template <typename... Args>
void trace(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (spdlog::logger *logger = ScopedSpdLogger::Get()) {
        logger->trace(fmt, std::forward<Args>(args)...);
    } else {
        fmt::print(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void info(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (spdlog::logger *logger = ScopedSpdLogger::Get()) {
        logger->info(fmt, std::forward<Args>(args)...);
    } else {
        fmt::print(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void debug(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (spdlog::logger *logger = ScopedSpdLogger::Get()) {
        logger->debug(fmt, std::forward<Args>(args)...);
    } else {
        fmt::print(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void warn(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (spdlog::logger *logger = ScopedSpdLogger::Get()) {
        logger->warn(fmt, std::forward<Args>(args)...);
    } else {
        fmt::print(fmt, std::forward<Args>(args)...);
    }
}

template <typename... Args>
void error(fmt::format_string<Args...> fmt, Args &&...args)
{
    if (spdlog::logger *logger = ScopedSpdLogger::Get()) {
        logger->error(fmt, std::forward<Args>(args)...);
    } else {
        fmt::print(stderr, fmt, std::forward<Args>(args)...);
    }
}

}  // namespace tamarindo::logging

#define TM_LOG_TRACE(...) ::tamarindo::logging::trace(__VA_ARGS__)
#define TM_LOG_INFO(...) ::tamarindo::logging::info(__VA_ARGS__)
#define TM_LOG_DEBUG(...) ::tamarindo::logging::debug(__VA_ARGS__)
#define TM_LOG_WARN(...) ::tamarindo::logging::warn(__VA_ARGS__)
#define TM_LOG_ERROR(...) ::tamarindo::logging::error(__VA_ARGS__)

#endif  // ENGINE_LIB_LOGGING_LOGGER_H_