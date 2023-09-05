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

#include "rendering_data.h"

namespace tamarindo
{

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

    SphericalCameraParams params;
    params.aspect_ratio = window_->AspectRatio();
    camera_ = std::make_unique<SphericalCamera>(params);

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

        camera_->OnUpdate(t);
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
