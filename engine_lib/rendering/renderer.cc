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

#include "engine_lib/rendering/renderer.h"

#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/render_state.h"
#include "engine_lib/rendering/window.h"
#include "engine_lib/utils/macros.h"
#include "engine_lib/rendering/solid_color_shader.h"
#include "engine_lib/rendering/vertex_input.h"

#include <d3d11.h>
#include <dxgi.h>
#include <stdlib.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace tamarindo
{

namespace
{

DXGI_SWAP_CHAIN_DESC SwapChainDesc(const Window& window)
{
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    // single back buffer.
    desc.BufferCount = 1;
    desc.BufferDesc.Width = window.Width();
    desc.BufferDesc.Height = window.Height();

    // Set regular 32-bit surface for the back buffer.
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // no vsync
    desc.BufferDesc.RefreshRate.Numerator = 0;
    desc.BufferDesc.RefreshRate.Denominator = 1;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = window.Handle();

    // multisampling off
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.Windowed = true;

    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    return desc;
}

D3D11_TEXTURE2D_DESC desc(const Window& window)
{
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Width = window.Width();
    desc.Height = window.Height();
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    return desc;
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;
    desc.Flags = 0;

    return desc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilDesc()
{
    D3D11_DEPTH_STENCIL_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.DepthEnable = true;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc = D3D11_COMPARISON_LESS;

    desc.StencilEnable = true;
    desc.StencilReadMask = 0xFF;
    desc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    return desc;
}

}  // namespace

/*static*/ std::unique_ptr<Renderer> Renderer::New(RenderState* render_state,
                                                   const Window& window)
{
    ComPtr<IDXGISwapChain> swap_chain;
    ComPtr<ID3D11RenderTargetView> render_target_view;
    ComPtr<ID3D11Texture2D> depth_stencil_buffer;
    ComPtr<ID3D11DepthStencilState> depth_stencil_state;
    ComPtr<ID3D11DepthStencilView> depth_stencil_view;

    // Create the DXGI swap chain
    {
        ComPtr<IDXGIFactory> factory;
        HRESULT res =
            CreateDXGIFactory(__uuidof(IDXGIFactory),
                              reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create DXGIFactory.");
            return nullptr;
        }

        res = factory->CreateSwapChain(&render_state->Device(),
                                       &SwapChainDesc(window),
                                       swap_chain.GetAddressOf());
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create swap chain.");
            return nullptr;
        }
    }

    {
        ComPtr<ID3D11Texture2D> back_buffer;
        HRESULT res = swap_chain->GetBuffer(
            0, __uuidof(ID3D11Texture2D), (LPVOID*)back_buffer.GetAddressOf());
        if (FAILED(res)) {
            return nullptr;
        }

        res = render_state->Device().CreateRenderTargetView(
            back_buffer.Get(), NULL, render_target_view.GetAddressOf());
        if (FAILED(res)) {
            return nullptr;
        }
    }

    HRESULT res = render_state->Device().CreateTexture2D(
        &desc(window), NULL, depth_stencil_buffer.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil buffer.");
        return nullptr;
    }

    res = render_state->Device().CreateDepthStencilView(
        depth_stencil_buffer.Get(), &DepthStencilViewDesc(),
        depth_stencil_view.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil view.");
        return nullptr;
    }

    res = render_state->Device().CreateDepthStencilState(
        &DepthStencilDesc(), depth_stencil_state.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil state.");
        return nullptr;
    }

    render_state->DeviceContext().OMSetRenderTargets(
        1, render_target_view.GetAddressOf(), depth_stencil_view.Get());

    render_state->DeviceContext().OMSetDepthStencilState(
        depth_stencil_state.Get(), 1);

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)window.Width();
    viewport.Height = (float)window.Height();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    render_state->DeviceContext().RSSetViewports(1, &viewport);

    return std::unique_ptr<Renderer>(new Renderer(
        render_state, swap_chain, render_target_view, depth_stencil_view));
}

Renderer::Renderer(RenderState* state, ComPtr<IDXGISwapChain> swap_chain,
                   ComPtr<ID3D11RenderTargetView> render_target_view,
                   ComPtr<ID3D11DepthStencilView> depth_stencil_view)
    : state_(state),
      swap_chain_(swap_chain),
      render_target_view_(render_target_view),
      depth_stencil_view_(depth_stencil_view)
{
}

Renderer::~Renderer() = default;

void Renderer::Render(const SolidColorShader& shader,
                      const ComPtr<ID3D11Buffer>& buffer)
{
    float background_color[4] = {0.678f, 0.749f, 0.796f, 1.0f};

    state_->DeviceContext().ClearRenderTargetView(render_target_view_.Get(),
                                                  background_color);
    state_->DeviceContext().ClearDepthStencilView(depth_stencil_view_.Get(),
                                                  D3D11_CLEAR_DEPTH, 1.0f, 0);

    state_->DeviceContext().IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = sizeof(VertexInput);
    UINT offset = 0;
    state_->DeviceContext().IASetVertexBuffers(0, 1, buffer.GetAddressOf(),
                                               &stride, &offset);

        state_->DeviceContext().IASetInputLayout(shader.input_layout_.Get());
    
        state_->DeviceContext().VSSetShader(shader.vertex_shader_.Get(), nullptr,
                                        0);
    state_->DeviceContext().PSSetShader(shader.pixel_shader_.Get(), nullptr, 0);

    state_->DeviceContext().Draw(3, 0);
    swap_chain_->Present(0, 0);
}

}  // namespace tamarindo
