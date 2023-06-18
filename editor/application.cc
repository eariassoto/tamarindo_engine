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
#include "camera/perspective_camera.h"

#include <memory>

namespace tamarindo
{

namespace
{

constexpr char SHADER_CODE[] = R"(
cbuffer ViewProjectionBuffer
{
    matrix viewProjection;
};

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

    output.position = mul(float4(input.position, 1.0f), viewProjection);
    output.color = input.color;

    return output;
}

float4 ps(PixelInput input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}

)";

constexpr float TRIANGLE_VB[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 1
    0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  // 2
    1.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f   // 3
};

constexpr unsigned int TRIANGLE_VB_STRIDE = sizeof(float) * 6;
constexpr unsigned int TRIANGLE_VB_SIZE = sizeof(TRIANGLE_VB);

}  // namespace

Application::Application(int window_show_behavior)
    : window_show_behavior_(window_show_behavior)
{
    window_ = Window::New(this);
    TM_ASSERT(window_);

    const bool res = RenderState::CreateUniqueInstance(*window_);
    TM_ASSERT(res);

    renderer_ = Renderer::New(*window_);

    std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0}};

    shader_ = Shader::New(SHADER_CODE, input_layout_desc);
    TM_ASSERT(shader_);

    PerspectiveCamera camera(XMConvertToRadians(90.0f), 800.f / 600.f, 0.1f,
                             1000.f);
    D3D11_BUFFER_DESC camera_buf_desc;
    camera_buf_desc.Usage = D3D11_USAGE_DYNAMIC;
    camera_buf_desc.ByteWidth = camera.GetBufferSize();
    camera_buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    camera_buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    camera_buf_desc.MiscFlags = 0;
    camera_buf_desc.StructureByteStride = 0;

    g_Device->CreateBuffer(&camera_buf_desc, nullptr,
                           camera_cb_.GetAddressOf());

    D3D11_MAPPED_SUBRESOURCE mapped_res;
    g_DeviceContext->Map(camera_cb_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
                         &mapped_res);
    XMMATRIX* data_ptr = static_cast<XMMATRIX*>(mapped_res.pData);

    // Update the view-projection matrix in the constant buffer
    // TODO: maybe smth like CopyBuffer
    *data_ptr = camera.GetViewProjectionMat();

    g_DeviceContext->Unmap(camera_cb_.Get(), 0);

    vertex_buffer_ =
        VertexBuffer::New(TRIANGLE_VB, TRIANGLE_VB_SIZE, TRIANGLE_VB_STRIDE);
    TM_ASSERT(vertex_buffer_);
}

Application ::~Application() { RenderState::DestroyUniqueInstance(); }

void Application::Run()
{
    TM_LOG_INFO("Starting application...");
    window_->Show(window_show_behavior_);

    MSG msg;
    while (is_running_) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer_->Render(1, camera_cb_.GetAddressOf(), *shader_,
                          *vertex_buffer_);
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
