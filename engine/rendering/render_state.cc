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

    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                   0, nullptr, 0, D3D11_SDK_VERSION,
                                   res->device.GetAddressOf(), &feature_level,
                                   res->device_context.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create DirectX11 device. Error: {}", hr);
        return nullptr;
    }

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
        hr =
            CreateDXGIFactory(__uuidof(IDXGIFactory),
                              reinterpret_cast<void**>(factory.GetAddressOf()));
        if (FAILED(hr)) {
            TM_LOG_ERROR("Could not create DirectXGI factory. Error: {}", hr);
            return nullptr;
        }

        hr = factory->CreateSwapChain(res->device.Get(), &desc,
                                      res->swap_chain.GetAddressOf());
        if (FAILED(hr)) {
            TM_LOG_ERROR("Could not create swap chain. Error: {}", hr);
            return nullptr;
        }
    }

    ComPtr<ID3D11Texture2D> render_target;

    hr = res->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                    (void**)render_target.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not get back buffer from swap chain. Error: {}",
                     hr);
        return nullptr;
    }

    ComPtr<ID3D11RenderTargetView> render_target_view;
    hr = res->device->CreateRenderTargetView(render_target.Get(), 0,
                                             render_target_view.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create render target view. Error: {}", hr);
        return nullptr;
    }

    D3D11_VIEWPORT viewport;
    viewport.Width = (float)desc.BufferDesc.Width;
    viewport.Height = (float)desc.BufferDesc.Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    res->device_context->RSSetViewports(1, &viewport);
    res->device_context->OMSetRenderTargets(
        1, render_target_view.GetAddressOf(), 0);

    return res;
}

RenderState::RenderState() = default;

RenderState::~RenderState() = default;

}  // namespace tamarindo
