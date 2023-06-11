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

#include "rendering/render_state.h"

#include "logging/logger.h"
#include "window/window.h"
#include "utils/macros.h"

#include <d3d11.h>

namespace tamarindo
{

/*static*/ std::unique_ptr<RenderState> RenderState::New(const Window& window)
{
    std::unique_ptr<RenderState> res = std::make_unique<RenderState>();

    /*D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    HRESULT res =
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                          nullptr, 0, D3D11_SDK_VERSION, device.GetAddressOf(),
                          &feature_level, device_context.GetAddressOf());

    if (FAILED(res)) {
        TM_LOG_ERROR("D3D11CreateDevice failed.");
        return nullptr;
    }

    TM_LOG_DEBUG("DirectX11 device context created.");*/

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // single back buffer.
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = window.Width();
    swapChainDesc.BufferDesc.Height = window.Height();

    // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // no vsync
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window.Handle();

    // multisampling off
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.Windowed = true;

    swapChainDesc.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
                      D3D11_SDK_VERSION, res->device.GetAddressOf(),
                      &feature_level, res->device_context.GetAddressOf());
    {
        ComPtr<IDXGIFactory> factory;
        CreateDXGIFactory(__uuidof(IDXGIFactory),
                          reinterpret_cast<void**>(factory.GetAddressOf()));

        factory->CreateSwapChain(res->device.Get(), &swapChainDesc,
                                 res->swap_chain.GetAddressOf());
    }

    ComPtr<ID3D11Texture2D> render_target;

    res->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                               (void**)render_target.GetAddressOf());

    ComPtr<ID3D11RenderTargetView> render_target_view;

    res->device->CreateRenderTargetView(render_target.Get(), 0,
                                        render_target_view.GetAddressOf());

    D3D11_VIEWPORT viewport = {0,
                               0,
                               (float)swapChainDesc.BufferDesc.Width,
                               (float)swapChainDesc.BufferDesc.Height,
                               0,
                               1};

    res->device_context->RSSetViewports(1, &viewport);
    res->device_context->OMSetRenderTargets(
        1, render_target_view.GetAddressOf(), 0);

    return res;
}

RenderState::RenderState() = default;

RenderState::~RenderState() = default;

}  // namespace tamarindo
