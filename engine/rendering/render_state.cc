/*
 Copyright 2023 Emmanuel Arias Soto

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
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include "rendering/render_state.h"

#include "logging/logger.h"
#include "window/window.h"
#include "utils/macros.h"

#include <d3d11.h>

namespace tamarindo
{

namespace
{

RenderState* g_render_state = nullptr;

}

bool RenderState::Initialize(unsigned int width, unsigned int height)
{
    TM_ASSERT(!g_render_state);

    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr =
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                          nullptr, 0, D3D11_SDK_VERSION, device.GetAddressOf(),
                          &feature_level, device_context.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create DirectX11 device. Error: {}", hr);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    // single back buffer.
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;

    // Set regular 32-bit surface for the back buffer.
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // no vsync
    desc.BufferDesc.RefreshRate.Numerator = 0;
    desc.BufferDesc.RefreshRate.Denominator = 1;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = Window::GetWindowHandle();

    // multisampling off
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.Windowed = true;

    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    {
        ComPtr<IDXGIFactory> factory;
        HRESULT res =
            CreateDXGIFactory(__uuidof(IDXGIFactory),
                              reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create DXGIFactory.");
            return false;
        }

        res = factory->CreateSwapChain(device.Get(), &desc,
                                       swap_chain.GetAddressOf());
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create swap chain.");
            return false;
        }
    }

    {
        ComPtr<ID3D11Texture2D> back_buffer;
        HRESULT res = swap_chain->GetBuffer(
            0, __uuidof(ID3D11Texture2D), (LPVOID*)back_buffer.GetAddressOf());
        if (FAILED(res)) {
            return false;
        }

        res = device->CreateRenderTargetView(back_buffer.Get(), NULL,
                                             render_target_view.GetAddressOf());
        if (FAILED(res)) {
            return false;
        }
    }

    ComPtr<ID3D11Texture2D> depth_stencil_buffer;
    {
        D3D11_TEXTURE2D_DESC texture_desc;
        ZeroMemory(&texture_desc, sizeof(texture_desc));

        texture_desc.Width = width;
        texture_desc.Height = height;
        texture_desc.MipLevels = 1;
        texture_desc.ArraySize = 1;
        texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texture_desc.CPUAccessFlags = 0;
        texture_desc.MiscFlags = 0;

        HRESULT res = device->CreateTexture2D(
            &texture_desc, NULL, depth_stencil_buffer.GetAddressOf());
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create depth/stencil buffer.");
            return false;
        }
    }

    D3D11_RASTERIZER_DESC rasterizer_desc = {};
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;

    ID3D11RasterizerState* rasterizer_state;
    device->CreateRasterizerState(&rasterizer_desc, &rasterizer_state);
    device_context->RSSetState(rasterizer_state);

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    ZeroMemory(&desc, sizeof(desc));

    depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;
    depth_stencil_view_desc.Flags = 0;

    HRESULT res = device->CreateDepthStencilView(
        depth_stencil_buffer.Get(), &depth_stencil_view_desc,
        depth_stencil_view.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil view.");
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC dep_stencil_desc;
    ZeroMemory(&dep_stencil_desc, sizeof(dep_stencil_desc));

    dep_stencil_desc.DepthEnable = true;
    dep_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dep_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

    dep_stencil_desc.StencilEnable = true;
    dep_stencil_desc.StencilReadMask = 0xFF;
    dep_stencil_desc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    dep_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dep_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dep_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dep_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    dep_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dep_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dep_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dep_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    ComPtr<ID3D11DepthStencilState> depth_stencil_state;
    res = device->CreateDepthStencilState(&dep_stencil_desc,
                                          depth_stencil_state.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil state.");
        return false;
    }

    device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(),
                                       depth_stencil_view.Get());

    device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    device_context->RSSetViewports(1, &viewport);

    device_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_render_state = this;
    return true;
}

/*static*/ void RenderState::Shutdown()
{
    TM_ASSERT(g_render_state);
    TM_ASSERT(g_render_state == this);
    g_render_state = nullptr;
}

/*static*/ RenderState* RenderState::Get()
{
    TM_ASSERT(g_render_state);
    return g_render_state;
}

RenderState::RenderState() = default;

RenderState::~RenderState() = default;

}  // namespace tamarindo
