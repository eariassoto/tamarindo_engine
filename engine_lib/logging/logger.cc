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

#include "engine_lib/logging/logger.h"

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
