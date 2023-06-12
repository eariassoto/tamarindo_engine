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
#include "application.h"

#include "logging/logger.h"
#include "utils/macros.h"
#include "window/window.h"
#include "rendering/shader.h"
#include "rendering/vertex_buffer.h"

#include <memory>

namespace tamarindo
{

namespace
{

constexpr char SHADER_CODE[] = R"(
struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

PixelInput vs(VertexInput input)
{
    PixelInput output;

    output.position = float4(input.position, 1.0f);
    output.color = input.color;

    return output;
}

float4 ps(PixelInput input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}

)";

constexpr float TRIANGLE_VB[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 1
    0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // 2
    0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // 3
};

constexpr unsigned int TRIANGLE_VB_STRIDE = sizeof(float) * 6;
constexpr unsigned int TRIANGLE_VB_SIZE = sizeof(TRIANGLE_VB);

bool g_is_running = true;

}  // namespace

Application::Application(int window_show_behavior)
    : window_show_behavior_(window_show_behavior)
{
    window_ = Window::New(this);
    TM_ASSERT(window_);

    render_state_ = RenderState::New(*window_);
    TM_ASSERT(render_state_);

    std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0}};

    std::unique_ptr<Shader> shader =
        Shader::New(render_state_.get(), SHADER_CODE, input_layout_desc);
    TM_ASSERT(shader);
    render_state_->device_context->IASetInputLayout(shader->input_layout.Get());
    render_state_->device_context->VSSetShader(shader->vertex_shader.Get(), 0,
                                               0);
    render_state_->device_context->PSSetShader(shader->pixel_shader.Get(), 0,
                                               0);

    std::unique_ptr<VertexBuffer> vertex_buffer = VertexBuffer::New(
        render_state_.get(), TRIANGLE_VB, TRIANGLE_VB_SIZE, TRIANGLE_VB_STRIDE);
    TM_ASSERT(vertex_buffer);

    UINT offset = 0;
    render_state_->device_context->IASetVertexBuffers(
        0, 1, vertex_buffer->buffer.GetAddressOf(), &TRIANGLE_VB_STRIDE,
        &offset);
    render_state_->device_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    render_state_->device_context->Draw(3, 0);
}

void Application::Run()
{
    TM_LOG_INFO("Starting application...");
    window_->Show(window_show_behavior_);

    MSG msg;
    while (g_is_running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        render_state_->swap_chain->Present(0, 0);
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
            g_is_running = false;
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

}  // namespace tamarindo
