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
namespace
{
void Render(const Shader& shader, const DirectX::XMMATRIX& transform,
            const MatrixConstantBuffer& constant_buffer,
            const ModelData& model_data, const RenderState& render_state)
{
    ID3D11DeviceContext* device_context = render_state.device_context.Get();

    device_context->ClearRenderTargetView(render_state.render_target_view.Get(),
                                          BACKGROUND_COLOR);
    device_context->ClearDepthStencilView(render_state.depth_stencil_view.Get(),
                                          D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Bind shader
    device_context->IASetInputLayout(&shader.input_layout());
    device_context->VSSetShader(&shader.vertex_shader(), 0, 0);
    device_context->PSSetShader(&shader.pixel_shader(), 0, 0);

    // Update transform uniform
    device_context->VSSetConstantBuffers(0, 1,
                                         constant_buffer.buffer.GetAddressOf());
    D3D11_MAPPED_SUBRESOURCE mapped_res;
    device_context->Map(constant_buffer.buffer.Get(), 0,
                        D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
    DirectX::XMMATRIX* data_ptr =
        static_cast<DirectX::XMMATRIX*>(mapped_res.pData);
    *data_ptr = transform;

    // Bind mesh
    auto stride = model_data.vertex_buffer_stride();
    auto vb_offset = model_data.vertex_buffer_offset();
    device_context->IASetVertexBuffers(
        0, 1, model_data.vertex_buffer.GetAddressOf(), &stride, &vb_offset);
    device_context->IASetIndexBuffer(model_data.index_buffer.Get(),
                                     DXGI_FORMAT_R32_UINT,
                                     model_data.index_buffer_offset());

    device_context->DrawIndexed(model_data.index_count, 0, 0);

    render_state.swap_chain->Present(0, 0);
}
}  // namespace

Application::Application()
{
    Window::Initialize(this);

    RenderState::Initialize(WIDTH, HEIGHT, &render_state);

    shader_ = ShaderBuilder::CompilePosUvShader(SHADER_CODE);
    TM_ASSERT(shader_);

    PerspectiveCameraParams perspective_params;
    perspective_params.aspect_ratio = ASPECT_RATIO;
    camera_ = std::make_unique<PerspectiveCamera>(perspective_params);

    camera_controller_ =
        std::make_unique<SphericalCameraController>(SphericalCameraParams());
    camera_->SetController(camera_controller_.get());

    mvp_cb_ = std::make_unique<MatrixConstantBuffer>();

    std::vector<float> vertex_data(TRIANGLE_VB.begin(), TRIANGLE_VB.end());
    std::vector<unsigned int> index_data(TRIANGLE_IB.begin(),
                                         TRIANGLE_IB.end());
    model_data_ = std::make_unique<ModelData>(vertex_data, index_data);
    TM_ASSERT(model_data_);
}

Application ::~Application() { RenderState::Shutdown(&render_state); }

void Application::Run()
{
    TM_LOG_INFO("Starting application...");
    Window::Show();

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
        model_matrix =
            XMMatrixTranspose(model_matrix * camera_->GetViewProjMat());
        Render(*shader_, model_matrix, *mvp_cb_, *model_data_, render_state);

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
