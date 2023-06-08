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
#include "engine_lib/rendering/buffers.h"
#include "engine_lib/rendering/vertex_input.h"

#include <DirectXMath.h>
#include <windows.h>

namespace tamarindo
{

namespace
{

constexpr char* CLASS_NAME = "TamarindoEditorClass";

constexpr VertexInput TRIANGLE_VERTICES[] = {
    {{-0.5f, -0.5f, 0.0f, 1.0f}},  // Vertex 1: Bottom-left
    {{0.5f, -0.5f, 0.0f, 1.0f}},   // Vertex 2: Bottom-right
    {{0.0f, 0.5f, 0.0f, 1.0f}}     // Vertex 3: Top-center
};

constexpr unsigned int VERTEX_COUNT = 3u;

}  // namespace

Application::Application(int window_show_behavior)
    : window_show_behavior_(window_show_behavior)
{
    window_ = Window::New(CLASS_NAME, this);

    render_state_ = RenderState::New(*window_);

    renderer_ = Renderer::New(render_state_.get(), *window_);

    shader_ = SolidColorShader::New(render_state_.get());

    vertex_buffer_ = CreateVertexBuffer(render_state_.get(), TRIANGLE_VERTICES,
                                        VERTEX_COUNT);
}

void Application::Run()
{
    TM_LOG_INFO("Starting application...");

    window_->Show(window_show_behavior_);
    window_->Update();

    MSG msg;
    while (is_running_) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer_->Render(*shader_, vertex_buffer_);
    }
}

void Application::Terminate()
{
    TM_LOG_INFO("Terminating application...");
    renderer_.reset();
    render_state_.reset();
    window_.reset();
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
