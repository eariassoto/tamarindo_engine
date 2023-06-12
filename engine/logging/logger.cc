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

#include "logger.h"
#include "utils/macros.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace
{
std::shared_ptr<spdlog::logger> g_SpdLogger = nullptr;
}

namespace tamarindo::logging
{

ScopedSpdLogger::ScopedSpdLogger()
{
    spdlog::set_pattern("[%T] %^[%l]%$ %n: %v");

    if (g_SpdLogger) {
        TM_BREAK();
    }
    g_SpdLogger = spdlog::stdout_color_mt("TM_CORE");
    g_SpdLogger->set_level(spdlog::level::trace);
}

ScopedSpdLogger::~ScopedSpdLogger()
{
    if (!g_SpdLogger) {
        TM_BREAK();
    }
    g_SpdLogger.reset();
}

/*static*/ spdlog::logger* ScopedSpdLogger::Get()
{
    // TODO: check if null
    return g_SpdLogger.get();
}

}  // namespace tamarindo::logging
