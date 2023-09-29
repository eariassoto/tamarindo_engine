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

Application::Application()
{
    tmrd::Window::Initialize(this);

    const auto window_data = GameData::GetWindowData();
    tmrd::RenderState::Initialize(window_data.width, window_data.height,
                                  &render_state_);

    shader_ = tmrd::ShaderBuilder::CompilePosUvShader(SHADER_CODE);
    TM_ASSERT(shader_);

    tmrd::PerspectiveCameraParams perspective_params;
    perspective_params.aspect_ratio = window_data.aspect_ratio;
    camera_ = std::make_unique<tmrd::PerspectiveCamera>(perspective_params);

    camera_controller_ = std::make_unique<tmrd::SphericalCameraController>(
        tmrd::SphericalCameraParams());
    camera_->SetController(camera_controller_.get());

    mvp_cb_ = std::make_unique<tmrd::MatrixConstantBuffer>();

    model_data_ = GameData::GetCubeModel();
    model_buffers_ = std::make_unique<tmrd::ModelData>(
        model_data_.vertex_buffer_data, model_data_.index_buffer_data);
    TM_ASSERT(model_buffers_);
}

Application ::~Application() { tmrd::RenderState::Shutdown(&render_state_); }

void Application::Run()
{
    TM_LOG_INFO("Starting application...");
    tmrd::Window::Show();

    BindScene();

    tmrd::Timer t;
    MSG msg;
    while (is_running_) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        t.StartFrame();

        Update(t);

        Render();

        keyboard_.ResetFrameKeyEvents();
    }
}

void Application::BindScene()
{
    ID3D11DeviceContext* device_context = render_state_.device_context.Get();
    // Bind scene constant buffer
    device_context->VSSetConstantBuffers(0, 1, mvp_cb_->buffer.GetAddressOf());

    // Bind shader
    device_context->IASetInputLayout(&shader_->input_layout());
    device_context->VSSetShader(&shader_->vertex_shader(), 0, 0);
    device_context->PSSetShader(&shader_->pixel_shader(), 0, 0);

    // Bind mesh
    auto stride = model_buffers_->vertex_buffer_stride();
    auto vb_offset = model_buffers_->vertex_buffer_offset();
    device_context->IASetVertexBuffers(
        0, 1, model_buffers_->vertex_buffer.GetAddressOf(), &stride,
        &vb_offset);
    device_context->IASetIndexBuffer(model_buffers_->index_buffer.Get(),
                                     DXGI_FORMAT_R32_UINT,
                                     model_buffers_->index_buffer_offset());
}

void Application::Update(const tmrd::Timer& t)
{
    camera_->OnUpdate(t);

    // transform_.SetScale(0.5 * sin(t.TotalTime()) + 1);
    transform_.AddRotationY(XM_PIDIV4 * t.DeltaTime());
    D3D11_MAPPED_SUBRESOURCE mapped_res;
    ID3D11DeviceContext* device_context = render_state_.device_context.Get();
    device_context->Map(mvp_cb_->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
                        &mapped_res);
    DirectX::XMMATRIX* data_ptr =
        static_cast<DirectX::XMMATRIX*>(mapped_res.pData);
    *data_ptr =
        XMMatrixTranspose(transform_.GetMatrix() * camera_->GetViewProjMat());
}

void Application::Render()
{
    ID3D11DeviceContext* device_context = render_state_.device_context.Get();
    device_context->ClearRenderTargetView(
        render_state_.render_target_view.Get(), BACKGROUND_COLOR);
    device_context->ClearDepthStencilView(
        render_state_.depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    for (int i = 0; i < model_data_.submodels.size(); ++i) {
        // Draw call
        const auto& submodel = model_data_.submodels[i];
        device_context->DrawIndexed(submodel.index_count, submodel.index_offset,
                                    submodel.vertex_offset);
    }

    render_state_.swap_chain->Present(0, 0);
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
