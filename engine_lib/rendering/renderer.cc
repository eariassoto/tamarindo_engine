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
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {0};

    // single back buffer.
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.BufferDesc.Width = window.Width();
    swap_chain_desc.BufferDesc.Height = window.Height();

    // Set regular 32-bit surface for the back buffer.
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // no vsync
    swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = window.Handle();

    // multisampling off
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

    swap_chain_desc.Windowed = true;

    swap_chain_desc.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    return swap_chain_desc;
}

D3D11_TEXTURE2D_DESC DepthBufferDesc(const Window& window)
{
    D3D11_TEXTURE2D_DESC depth_buffer_desc = {0};
    depth_buffer_desc.Width = window.Width();
    depth_buffer_desc.Height = window.Height();
    depth_buffer_desc.MipLevels = 1;
    depth_buffer_desc.ArraySize = 1;
    depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_buffer_desc.SampleDesc.Count = 1;
    depth_buffer_desc.SampleDesc.Quality = 0;
    depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_buffer_desc.CPUAccessFlags = 0;
    depth_buffer_desc.MiscFlags = 0;

    return depth_buffer_desc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilDesc()
{
    D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = {0};

    depth_stencil_desc.DepthEnable = true;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

    depth_stencil_desc.StencilEnable = true;
    depth_stencil_desc.StencilReadMask = 0xFF;
    depth_stencil_desc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    return depth_stencil_desc;
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    depthStencilViewDesc.Flags = 0;

    return depthStencilViewDesc;
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

    if (!render_target_view) {
        TM_LOG_ERROR("Could not get render target view.");
        return nullptr;
    }

    HRESULT res = render_state->Device().CreateTexture2D(
        &DepthBufferDesc(window), NULL, depth_stencil_buffer.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil buffer.");
        return nullptr;
    }

    res = render_state->Device().CreateDepthStencilState(
        &DepthStencilDesc(), depth_stencil_state.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil state.");
        return nullptr;
    }

    render_state->DeviceContext().OMSetDepthStencilState(
        depth_stencil_state.Get(), 1);

    res = render_state->Device().CreateDepthStencilView(
        depth_stencil_buffer.Get(), &DepthStencilViewDesc(),
        depth_stencil_view.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil view.");
        return nullptr;
    }

    render_state->DeviceContext().OMSetRenderTargets(
        1, render_target_view.GetAddressOf(), depth_stencil_view.Get());

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)window.Width();
    viewport.Height = (float)window.Height();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    render_state->DeviceContext().RSSetViewports(1, &viewport);

    return std::unique_ptr<Renderer>(new Renderer(
        render_state, swap_chain, render_target_view, depth_stencil_buffer,
        depth_stencil_state, depth_stencil_view));
}

Renderer::Renderer(RenderState* state, ComPtr<IDXGISwapChain> swap_chain,
                   ComPtr<ID3D11RenderTargetView> render_target_view,
                   ComPtr<ID3D11Texture2D> depth_stencil_buffer,
                   ComPtr<ID3D11DepthStencilState> depth_stencil_state,
                   ComPtr<ID3D11DepthStencilView> depth_stencil_view)
    : state_(state),
      swap_chain_(swap_chain),
      render_target_view_(render_target_view),
      depth_stencil_buffer_(depth_stencil_buffer),
      depth_stencil_state_(depth_stencil_state),
      depth_stencil_view_(depth_stencil_view)
{
}

Renderer::~Renderer() {}

void Renderer::Render()
{
    float background_color[4] = {0.678f, 0.749f, 0.796f, 1.0f};

    state_->DeviceContext().ClearRenderTargetView(render_target_view_.Get(),
                                                  background_color);
    state_->DeviceContext().ClearDepthStencilView(depth_stencil_view_.Get(),
                                                  D3D11_CLEAR_DEPTH, 1.0f, 0);
    swap_chain_->Present(0, 0);
}

}  // namespace tamarindo
