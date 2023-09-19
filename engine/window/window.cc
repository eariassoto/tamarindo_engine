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

namespace tamarindo::Window
{

namespace
{

HWND window_handle = nullptr;

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

bool Initialize(WindowEventHandler* window_event_handler)
{
    const HINSTANCE& h_instance = GetModuleHandle(0);
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = h_instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND window = CreateWindowEx(0,                    // Optional window styles
                                 CLASS_NAME,           // Window class name
                                 WINDOW_TITLE,         // Window title
                                 WS_OVERLAPPEDWINDOW,  // Window style

                                 // Size and position
                                 CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

                                 NULL,        // Parent window
                                 NULL,        // Menu
                                 h_instance,  // Instance handle
                                 NULL         // Additional application data
    );
    if (window == nullptr) {
        TM_LOG_ERROR("Window creation failed.");
        return false;
    }
    window_handle = window;

    SetWindowLongPtr(window, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(window_event_handler));
    return true;
}

HWND GetWindowHandle() { return window_handle; }

void Show() { ShowWindow(window_handle, SW_SHOWNORMAL); }

}  // namespace tamarindo::Window
