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

#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/macros.h"

#include <memory>
#include <stdio.h>
#include <windows.h>

namespace
{

std::shared_ptr<spdlog::logger> g_SpdLogger = nullptr;

FILE* g_WinConsoleStream;

void InitializeWinConsole()
{
    AllocConsole();
    AttachConsole(ATTACH_PARENT_PROCESS);
    // TODO: Check for error
    freopen_s(&g_WinConsoleStream, "CONOUT$", "w", stdout);
    freopen_s(&g_WinConsoleStream, "CONOUT$", "w", stderr);
}

void ShutdownWinConsole()
{
    fclose(g_WinConsoleStream);
    FreeConsole();
}

}  // namespace

namespace tamarindo::logging
{

spdlog::logger* GetLogger() { return g_SpdLogger.get(); }

void Initialize()
{
    InitializeWinConsole();

    TM_ASSERT(!g_SpdLogger);
    spdlog::set_pattern("[%T] %^[%l]%$ %n: %v");

    g_SpdLogger = spdlog::stdout_color_mt("TM_CORE");
    g_SpdLogger->set_level(spdlog::level::trace);
}

void Shutdown()
{
    TM_ASSERT(g_SpdLogger);
    g_SpdLogger.reset();

    ShutdownWinConsole();
}

}  // namespace tamarindo::logging
