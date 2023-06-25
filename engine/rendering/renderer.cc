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

#include "rendering/renderer.h"

#include "logging/logger.h"
#include "rendering/render_state.h"
#include "rendering/shader.h"
#include "rendering/model.h"
#include "window/window.h"

#include <d3d11.h>
#include <dxgi.h>

namespace tamarindo
{

/*static*/ std::unique_ptr<Renderer> Renderer::New(const Window& window)
{
    ComPtr<IDXGISwapChain> swap_chain;
    ComPtr<ID3D11RenderTargetView> render_target_view;
    ComPtr<ID3D11Texture2D> depth_stencil_buffer;
    ComPtr<ID3D11DepthStencilState> depth_stencil_state;
    ComPtr<ID3D11DepthStencilView> depth_stencil_view;

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

    {
        ComPtr<IDXGIFactory> factory;
        HRESULT res =
            CreateDXGIFactory(__uuidof(IDXGIFactory),
                              reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create DXGIFactory.");
            return nullptr;
        }

        res = factory->CreateSwapChain(g_Device, &desc,
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

        res = g_Device->CreateRenderTargetView(
            back_buffer.Get(), NULL, render_target_view.GetAddressOf());
        if (FAILED(res)) {
            return nullptr;
        }
    }

    {
        D3D11_TEXTURE2D_DESC texture_desc;
        ZeroMemory(&texture_desc, sizeof(texture_desc));

        texture_desc.Width = window.Width();
        texture_desc.Height = window.Height();
        texture_desc.MipLevels = 1;
        texture_desc.ArraySize = 1;
        texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texture_desc.CPUAccessFlags = 0;
        texture_desc.MiscFlags = 0;

        HRESULT res = g_Device->CreateTexture2D(
            &texture_desc, NULL, depth_stencil_buffer.GetAddressOf());
        if (FAILED(res)) {
            TM_LOG_ERROR("Could not create depth/stencil buffer.");
            return nullptr;
        }
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
    ZeroMemory(&desc, sizeof(desc));

    depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;
    depth_stencil_view_desc.Flags = 0;

    HRESULT res = g_Device->CreateDepthStencilView(
        depth_stencil_buffer.Get(), &depth_stencil_view_desc,
        depth_stencil_view.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil view.");
        return nullptr;
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

    res = g_Device->CreateDepthStencilState(&dep_stencil_desc,
                                            depth_stencil_state.GetAddressOf());
    if (FAILED(res)) {
        TM_LOG_ERROR("Could not create depth/stencil state.");
        return nullptr;
    }

    g_DeviceContext->OMSetRenderTargets(1, render_target_view.GetAddressOf(),
                                        depth_stencil_view.Get());

    g_DeviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 1);

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)window.Width();
    viewport.Height = (float)window.Height();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    g_DeviceContext->RSSetViewports(1, &viewport);

    g_DeviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return std::unique_ptr<Renderer>(
        new Renderer(swap_chain, render_target_view, depth_stencil_view));
}

Renderer::Renderer(ComPtr<IDXGISwapChain> swap_chain,
                   ComPtr<ID3D11RenderTargetView> render_target_view,
                   ComPtr<ID3D11DepthStencilView> depth_stencil_view)
    : swap_chain_(swap_chain),
      render_target_view_(render_target_view),
      depth_stencil_view_(depth_stencil_view)
{
}

Renderer::~Renderer() = default;

void Renderer::Render(const Shader& shader, const Model& model)
{
    float background_color[4] = {0.678f, 0.749f, 0.796f, 1.0f};

    g_DeviceContext->ClearRenderTargetView(render_target_view_.Get(),
                                           background_color);
    g_DeviceContext->ClearDepthStencilView(depth_stencil_view_.Get(),
                                           D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Bind shader
    shader.Bind();

    // Bind mesh
    model.BindAndDraw();

    swap_chain_->Present(0, 0);
}

}  // namespace tamarindo
