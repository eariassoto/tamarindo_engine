/*
 Copyright 2024 Emmanuel Arias Soto

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

#include "scoped_windows_console.h"

#include <stdio.h>

#include <windows.h>

#include "utils/macros.h"

namespace tamarindo
{
namespace
{

bool is_console_open = false;

}

ScopedWindowsConsole::ScopedWindowsConsole()
{
    TM_ASSERT(!is_console_open);
    AllocConsole();
    AttachConsole(ATTACH_PARENT_PROCESS);
    freopen_s(&console_stream_, "CONOUT$", "w", stdout);
    freopen_s(&console_stream_, "CONOUT$", "w", stderr);

    is_console_open = true;
}

ScopedWindowsConsole::~ScopedWindowsConsole()
{
    TM_ASSERT(is_console_open);
    fclose(console_stream_);
    FreeConsole();

    is_console_open = false;
}

}  // namespace tamarindo
