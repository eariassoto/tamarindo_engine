
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

#include "engine_lib/rendering/window.h"

#include "engine_lib/rendering/window_event_handler.h"
#include "engine_lib/logging/logger.h"

namespace tamarindo
{

namespace
{
constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;
}  // namespace

namespace
{
LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Access instance through user data window long pointer
    auto instance = reinterpret_cast<tamarindo::WindowEventHandler*>(
        GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (instance != nullptr) {
        return instance->HandleWindowMessage(hWnd, message, wParam, lParam);
    } else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

}  // namespace

/*static*/ std::unique_ptr<Window> Window::CreateWithClass(
    const std::string& class_name, WindowEventHandler* window_event_handler)
{
    const HINSTANCE& h_instance = GetModuleHandle(0);
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = h_instance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = class_name.c_str();

    if (!RegisterClassEx(&wc)) {
        TM_LOG_ERROR("Window registration failed.");
        return nullptr;
    }

    HWND hwnd =
        CreateWindowEx(0, class_name.c_str(), "Tamarindo Editor",
                       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH,
                       HEIGHT, nullptr, nullptr, h_instance, nullptr);

    if (hwnd == nullptr) {
        TM_LOG_ERROR("Window creation failed.");
        return nullptr;
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(window_event_handler));

    return std::unique_ptr<Window>(new Window(hwnd, WIDTH, HEIGHT));
}

Window::~Window() = default;

Window::Window(HWND window_handle, unsigned int width, unsigned int height)
    : window_handle_(window_handle), width_(width), height_(height)
{
}

void Window::Show(int show_behavior)
{
    ShowWindow(window_handle_, show_behavior);
}

void Window::Update() { UpdateWindow(window_handle_); }

unsigned int Window::Width() const { return width_; }

unsigned int Window::Height() const { return height_; }

HWND Window::Handle() const { return window_handle_; }

}  // namespace tamarindo
