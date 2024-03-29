
/*
 Copyright 2023 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_WINDOW_WINDOW_H_
#define ENGINE_LIB_WINDOW_WINDOW_H_

#include <windows.h>

#include <memory>
#include <string>

namespace tamarindo
{

class WindowEventHandler;

namespace Window
{

struct InitParams {
    std::wstring class_name = L"TamarindoEditorClass";
    std::wstring window_title = L"Tamarindo Editor";
    int width = 800;
    int height = 600;
};

bool Initialize(InitParams const& init_params);

void SetEventHandler(WindowEventHandler* window_event_handler);

HWND GetWindowHandle();

void Show();

}  // namespace Window

}  // namespace tamarindo

#endif  // ENGINE_LIB_WINDOW_WINDOW_H_
