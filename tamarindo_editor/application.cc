/*
 Copyright 2021-2023 Emmanuel Arias Soto

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

#include "tamarindo_editor/application.h"

#include "engine_lib/logging/logger.h"

#include <windows.h>

namespace tamarindo
{

namespace
{
constexpr char* CLASS_NAME = "TamarindoEditorClass";
}  // namespace

/*static*/ Application* Application::CreateNew(const HINSTANCE& hInstance,
                                               int window_show_behavior)
{
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wc)) {
        TM_LOG_ERROR("Window registration failed.");
        return nullptr;
    }

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Tamarindo Editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr) {
        TM_LOG_ERROR("Window creation failed.");
        return nullptr;
    }

    Application* app = new Application(hwnd, window_show_behavior);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));

    return app;
}

Application::Application(HWND window_handle, int window_show_behavior)
    : window_handle_(window_handle), window_show_behavior_(window_show_behavior)
{
}

void Application::Run()
{
    ShowWindow(window_handle_, window_show_behavior_);
    UpdateWindow(window_handle_);

    MSG msg;
    while (is_running_) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Application::Terminate() { TM_LOG_DEBUG("Terminating application"); }

/*static*/ LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT message,
                                                    WPARAM wParam,
                                                    LPARAM lParam)
{
    // Access instance through user data window long pointer
    auto instance =
        reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (instance != nullptr) {
        return instance->HandleWindowMessage(hWnd, message, wParam, lParam);
    } else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

LRESULT Application::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam,
                                         LPARAM lParam)
{
    switch (message) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            is_running_ = false;
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

}  // namespace tamarindo
