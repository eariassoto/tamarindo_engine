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

#include "window/window.h"

#include "logging/logger.h"
#include "window/window_event_handler.h"
#include "utils/macros.h"

namespace tamarindo::Window
{

namespace
{

HWND g_WindowHandle = nullptr;

constexpr wchar_t CLASS_NAME[] = L"TamarindoEditorClass";
constexpr wchar_t WINDOW_TITLE[] = L"Tamarindo Editor";

LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    auto instance = reinterpret_cast<tamarindo::WindowEventHandler*>(
        GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (instance != nullptr) {
        return instance->HandleWindowMessage(hWnd, message, wParam, lParam);
    } else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
}  // namespace

bool Initialize(InitParams const& init_params)
{
    const HINSTANCE& h_instance = GetModuleHandle(0);
    WNDCLASS window_class = {};

    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = h_instance;
    window_class.lpszClassName = CLASS_NAME;

    RegisterClass(&window_class);

    HWND window = CreateWindowEx(0,  // Optional styles
                                 init_params.class_name.c_str(),  // class name
                                 init_params.window_title.c_str(),  // title
                                 WS_OVERLAPPEDWINDOW,               // style

                                 CW_USEDEFAULT, CW_USEDEFAULT,  // Position
                                 init_params.width, init_params.height,  // Size

                                 NULL,        // Parent window
                                 NULL,        // Menu
                                 h_instance,  // Instance handle
                                 NULL         // Additional application data
    );
    if (window == nullptr) {
        TM_LOG_ERROR("Window creation failed.");
        return false;
    }
    g_WindowHandle = window;
    return true;
}

void SetEventHandler(WindowEventHandler* window_event_handler)
{
    TM_ASSERT(g_WindowHandle);
    SetWindowLongPtr(g_WindowHandle, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(window_event_handler));
}

HWND GetWindowHandle() { return g_WindowHandle; }

void Show() { ShowWindow(g_WindowHandle, SW_SHOWNORMAL); }

}  // namespace tamarindo::Window
