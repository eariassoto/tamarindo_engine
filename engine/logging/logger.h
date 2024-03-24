/*
 Copyright 2021-2024 Emmanuel Arias Soto

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

#include <memory>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "logging/scoped_windows_console.h"

namespace tamarindo
{

class Logger
{
   public:
    static Logger *Get();

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    Logger();
    ~Logger();

    template <typename... Args>
    void Trace(fmt::format_string<Args...> fmt, Args &&...args)
    {
        logger_->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Info(fmt::format_string<Args...> fmt, Args &&...args)
    {
        logger_->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Debug(fmt::format_string<Args...> fmt, Args &&...args)
    {
        logger_->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Warn(fmt::format_string<Args...> fmt, Args &&...args)
    {
        logger_->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Error(fmt::format_string<Args...> fmt, Args &&...args)
    {
        logger_->error(fmt, std::forward<Args>(args)...);
    }

   private:
    ScopedWindowsConsole win_console_;
    std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace tamarindo

#define TM_LOG_TRACE(...) ::tamarindo::Logger::Get()->Trace(__VA_ARGS__)
#define TM_LOG_INFO(...) ::tamarindo::Logger::Get()->Info(__VA_ARGS__)
#define TM_LOG_DEBUG(...) ::tamarindo::Logger::Get()->Debug(__VA_ARGS__)
#define TM_LOG_WARN(...) ::tamarindo::Logger::Get()->Warn(__VA_ARGS__)
#define TM_LOG_ERROR(...) ::tamarindo::Logger::Get()->Error(__VA_ARGS__)

#endif  // ENGINE_LIB_LOGGING_LOGGER_H_
