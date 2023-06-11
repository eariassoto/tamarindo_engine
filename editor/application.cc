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
    std::unique_ptr<Window> window = Window::New(this);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // single back buffer.
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = window->Width();
    swapChainDesc.BufferDesc.Height = window->Height();

    // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // no vsync
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window->Handle();

    // multisampling off
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.Windowed = true;

    swapChainDesc.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> device_context;

    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
                      D3D11_SDK_VERSION, device.GetAddressOf(), &feature_level,
                      device_context.GetAddressOf());
    {
        ComPtr<IDXGIFactory> factory;
        CreateDXGIFactory(__uuidof(IDXGIFactory),
                          reinterpret_cast<void**>(factory.GetAddressOf()));

        factory->CreateSwapChain(device.Get(), &swapChainDesc,
                                 swap_chain_.GetAddressOf());
    }

    ComPtr<ID3D11Texture2D> render_target;

    swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
                           (void**)render_target.GetAddressOf());

    ComPtr<ID3D11RenderTargetView> render_target_view;

    device->CreateRenderTargetView(render_target.Get(), 0,
                                   render_target_view.GetAddressOf());

    D3D11_VIEWPORT viewport = {0,
                               0,
                               (float)swapChainDesc.BufferDesc.Width,
                               (float)swapChainDesc.BufferDesc.Height,
                               0,
                               1};

    device_context->RSSetViewports(1, &viewport);
    device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), 0);

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

    device->CreateVertexShader(cso->GetBufferPointer(), cso->GetBufferSize(), 0,
                               vertex_shader.GetAddressOf());

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0}};

    UINT numElements = ARRAYSIZE(inputLayoutDesc);

    ID3D11InputLayout* inputLayout;
    HRESULT res = device->CreateInputLayout(inputLayoutDesc, numElements,
                                            cso->GetBufferPointer(),
                                            cso->GetBufferSize(), &inputLayout);

    D3DCompile(SHADER_CODE, sizeof(SHADER_CODE) - 1, nullptr, nullptr, nullptr,
               "ps", "ps_5_0", shader_flags, 0, cso.ReleaseAndGetAddressOf(),
               &errorBlob);

    ComPtr<ID3D11PixelShader> pixel_shader;

    device->CreatePixelShader(cso->GetBufferPointer(), cso->GetBufferSize(), 0,
                              pixel_shader.GetAddressOf());

    device_context->VSSetShader(vertex_shader.Get(), 0, 0);

    device_context->PSSetShader(pixel_shader.Get(), 0, 0);

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
    device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

    UINT offset = 0;
    device_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    device_context->IASetInputLayout(inputLayout);
    device_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    device_context->Draw(3, 0);

    window->Show(window_show_behavior);
}

void Application::Run()
{
    TM_LOG_INFO("Starting application...");

    MSG msg;
    while (g_is_running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        swap_chain_->Present(0, 0);
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
