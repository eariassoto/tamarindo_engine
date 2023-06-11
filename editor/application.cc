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
#pragma comment(lib, "user32")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi.lib")

#include "application.h"

#include "logging/logger.h"
#include "utils/macros.h"
#include "window/window.h"

#include <DirectXMath.h>
#include <windows.h>
#include <d3dcompiler.h>

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

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 1
    0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,  // 2
    0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // 3
};

bool g_is_running = true;

constexpr D3D_FEATURE_LEVEL feature_levels[] = {D3D_FEATURE_LEVEL_11_0};

}  // namespace

Application::Application(int window_show_behavior)
    : window_show_behavior_(window_show_behavior)
{
    window_ = Window::New(this);
    TM_ASSERT(window_);

    render_state_ = RenderState::New(*window_);
    TM_ASSERT(render_state_);

    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> cso;

    ID3DBlob* errorBlob = nullptr;
    D3DCompile(SHADER_CODE, sizeof(SHADER_CODE) - 1, nullptr, nullptr, nullptr,
               "vs", "vs_5_0", shader_flags, 0, cso.GetAddressOf(), &errorBlob);
    if (errorBlob) {
        char* errorMessage = static_cast<char*>(errorBlob->GetBufferPointer());
        errorBlob->Release();
    }

    ComPtr<ID3D11VertexShader> vertex_shader;

    render_state_->device->CreateVertexShader(cso->GetBufferPointer(),
                                              cso->GetBufferSize(), 0,
                                              vertex_shader.GetAddressOf());

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0}};

    UINT numElements = ARRAYSIZE(inputLayoutDesc);

    ID3D11InputLayout* inputLayout;
    HRESULT res = render_state_->device->CreateInputLayout(
        inputLayoutDesc, numElements, cso->GetBufferPointer(),
        cso->GetBufferSize(), &inputLayout);

    D3DCompile(SHADER_CODE, sizeof(SHADER_CODE) - 1, nullptr, nullptr, nullptr,
               "ps", "ps_5_0", shader_flags, 0, cso.ReleaseAndGetAddressOf(),
               &errorBlob);

    ComPtr<ID3D11PixelShader> pixel_shader;

    render_state_->device->CreatePixelShader(cso->GetBufferPointer(),
                                             cso->GetBufferSize(), 0,
                                             pixel_shader.GetAddressOf());

    render_state_->device_context->VSSetShader(vertex_shader.Get(), 0, 0);

    render_state_->device_context->PSSetShader(pixel_shader.Get(), 0, 0);

    UINT stride =
        sizeof(float) * 6;  // 6 components (3 for position, 3 for color)
    UINT bufferSize = sizeof(vertices);

    // Create the vertex buffer
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = bufferSize;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    ID3D11Buffer* vertexBuffer;
    render_state_->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData,
                                        &vertexBuffer);

    UINT offset = 0;
    render_state_->device_context->IASetVertexBuffers(0, 1, &vertexBuffer,
                                                      &stride, &offset);
    render_state_->device_context->IASetInputLayout(inputLayout);
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
