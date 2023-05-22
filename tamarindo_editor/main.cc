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

#include <windows.h>

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//                    LPSTR lpCmdLine, int nCmdShow)
// {
//     // Step 1: Register the window class
//     const char* class_name = "TamarindoEditorClass";

//     WNDCLASSEX wc{};
//     wc.cbSize = sizeof(WNDCLASSEX);
//     wc.style = CS_HREDRAW | CS_VREDRAW;
//     wc.lpfnWndProc = DefWindowProc;
//     wc.hInstance = hInstance;
//     wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//     wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//     wc.lpszClassName = class_name;

//     if (!RegisterClassEx(&wc)) {
//         MessageBox(nullptr, "Window registration failed", "Error",
//                    MB_OK | MB_ICONERROR);
//         return -1;
//     }

//     // Step 2: Create the window
//     HWND hwnd = CreateWindowEx(0, class_name, "My Window",
//     WS_OVERLAPPEDWINDOW,
//                                CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
//                                nullptr, nullptr, hInstance, nullptr);

//     if (hwnd == nullptr) {
//         MessageBox(nullptr, "Window creation failed", "Error",
//                    MB_OK | MB_ICONERROR);
//         return -1;
//     }

//     // Step 3: Show and update the window
//     ShowWindow(hwnd, nCmdShow);
//     UpdateWindow(hwnd);

//     // Step 4: Message loop
//     MSG msg{};
//     while (GetMessage(&msg, nullptr, 0, 0)) {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     return static_cast<int>(msg.wParam);
// }

#include "engine_lib/logging/logger.h"
#include "tamarindo_editor/application.h"

using namespace tamarindo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    logging::ScopedSpdLogger logger;

    Application* app = Application::CreateNew(hInstance, nCmdShow);

    if (!app) {
        TM_LOG_ERROR("Could not initialize application.", errno);
        return -1;
    }

    TM_LOG_INFO("Starting application...");
    app->Run();
    app->Terminate();
    TM_LOG_INFO("Done");

    return 0;
}
