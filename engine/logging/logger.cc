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

#include "logger.h"

#include <memory>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/macros.h"

namespace tamarindo
{

namespace
{

Logger* g_Logger = nullptr;

}  // namespace

// static
Logger* Logger::Get()
{
    TM_ASSERT(g_Logger);
    return g_Logger;
}

Logger::Logger() : logger_(spdlog::stdout_color_st("TAMARINDO"))
{
    TM_ASSERT(!g_Logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^[%l]%$ %v");
    logger_->set_level(spdlog::level::trace);
    g_Logger = this;
}

Logger::~Logger()
{
    TM_ASSERT(g_Logger);
    g_Logger = nullptr;
}

}  // namespace tamarindo
