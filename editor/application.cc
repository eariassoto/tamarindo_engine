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
#include "rendering/shader_builder.h"
#include "rendering/model.h"
#include "utils/timer.h"

#include <memory>
#include <cmath>

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
    float2 tex : TEX;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEX;
};

PixelInput vs(VertexInput input)
{
    PixelInput output;

    output.position = mul(float4(input.position, 1.0f), viewProjection);
    output.tex = input.tex;

    return output;
}

float4 ps(PixelInput input) : SV_TARGET
{
    return float4(input.tex, 0.0f, 1.0f);
}

)";

}  // namespace

Application::Application(int window_show_behavior)
    : window_show_behavior_(window_show_behavior)
{
    window_ = Window::New(this);
    TM_ASSERT(window_);

    const bool res = RenderState::CreateUniqueInstance(*window_);
    TM_ASSERT(res);

    renderer_ = Renderer::New(*window_);

    shader_ = ShaderBuilder::CompilePosUvShader(SHADER_CODE);
    TM_ASSERT(shader_);

    PerspectiveCameraParams params;
    params.eye = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
    params.at = XMVectorZero();
    params.aspect_ratio = 800.f / 600.f;

    camera_ = std::unique_ptr<PerspectiveCamera>(new PerspectiveCamera(params));

    D3D11_BUFFER_DESC camera_buf_desc;
    camera_buf_desc.Usage = D3D11_USAGE_DYNAMIC;
    camera_buf_desc.ByteWidth = camera_->GetBufferSize();
    camera_buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    camera_buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    camera_buf_desc.MiscFlags = 0;
    camera_buf_desc.StructureByteStride = 0;

    g_Device->CreateBuffer(&camera_buf_desc, nullptr,
                           camera_cb_.GetAddressOf());
    // Bind constant buffers
    g_DeviceContext->VSSetConstantBuffers(0, 1, camera_cb_.GetAddressOf());

    model_ = Model::NewTriangleModel();
    TM_ASSERT(model_);
}

Application ::~Application() { RenderState::DestroyUniqueInstance(); }

void Application::Run()
{
    TM_LOG_INFO("Starting application...");
    window_->Show(window_show_behavior_);

    Timer t;
    MSG msg;
    while (is_running_) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        t.StartFrame();

        D3D11_MAPPED_SUBRESOURCE mapped_res;
        g_DeviceContext->Map(camera_cb_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
                             &mapped_res);
        XMMATRIX* data_ptr = static_cast<XMMATRIX*>(mapped_res.pData);

        const double scale_factor = 0.5 * sin(t.TotalTime()) + 1;
        DirectX::XMMATRIX model_matrix =
            DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) *
            DirectX::XMMatrixTranslation(0, 0, 0);

        *data_ptr = camera_->GetViewProjectionMat() * model_matrix;
        g_DeviceContext->Unmap(camera_cb_.Get(), 0);

        renderer_->Render(*shader_, *model_);
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
