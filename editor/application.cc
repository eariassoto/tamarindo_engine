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
#include "utils/timer.h"

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

constexpr unsigned int MODEL_STRIDE = sizeof(float) * 5;
constexpr float TRIANGLE_VB[] = {
    // Front face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 2
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 3

    // Right face
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 4
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 5
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 6
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 7

    // Back face
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 8
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 9
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 10
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 11

    // Left face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 12
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 13
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 14
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 15

    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // 16
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   // 17
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 18
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // 19

    // Bottom face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 20
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 21
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 22
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f  // 23
};
constexpr unsigned int TRIANGLE_VB_SIZE = sizeof(TRIANGLE_VB);

constexpr unsigned int TRIANGLE_IB[] = {
    0,  1,  2,  2,  3,  0,   // Front face
    4,  5,  6,  6,  7,  4,   // Right face
    8,  9,  10, 10, 11, 8,   // Back face
    12, 13, 14, 14, 15, 12,  // Left face
    16, 17, 18, 18, 19, 16,  // Top face
    20, 21, 22, 22, 23, 20   // Bottom face
};
constexpr unsigned int TRIANGLE_IB_SIZE = sizeof(TRIANGLE_IB);
constexpr unsigned int TRIANGLE_IB_COUNT =
    TRIANGLE_IB_SIZE / sizeof(unsigned int);

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
    params.aspect_ratio = window_->AspectRatio();
    camera_ = std::make_unique<PerspectiveCamera>(params);

    mvp_cb_ = std::make_unique<MatrixConstantBuffer>();
    g_DeviceContext->VSSetConstantBuffers(0, 1, mvp_cb_->Buffer());

    vb_ = std::make_unique<VertexBuffer>(MODEL_STRIDE, (void*)TRIANGLE_VB,
                                         TRIANGLE_VB_SIZE);
    TM_ASSERT(vb_);

    ib_ = std::make_unique<IndexBuffer>(TRIANGLE_IB_COUNT, (void*)TRIANGLE_IB,
                                        TRIANGLE_IB_SIZE);
    TM_ASSERT(ib_);
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

        const double scale_factor = 0.5 * sin(t.TotalTime()) + 1;
        DirectX::XMMATRIX model_matrix =
            DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) *
            DirectX::XMMatrixTranslation(0, 0, 0);
        mvp_cb_->UpdateData(
            XMMatrixTranspose(XMMatrixIdentity() * camera_->GetViewMat() *
                              camera_->GetProjectionMat()));

        renderer_->Render(*shader_, *vb_, *ib_);

        keyboard_.ResetFrameKeyEvents();
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

        case WM_KEYDOWN:
        case WM_KEYUP:
            keyboard_.KeyCallback((int)wParam, message == WM_KEYDOWN);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

}  // namespace tamarindo
