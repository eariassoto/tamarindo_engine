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

/*static*/ bool RenderState::CreateUniqueInstance(const Window& window)
{
    TM_ASSERT(!g_render_state);
    RenderState* res = new RenderState();

    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                   0, nullptr, 0, D3D11_SDK_VERSION,
                                   res->device.GetAddressOf(), &feature_level,
                                   res->device_context.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create DirectX11 device. Error: {}", hr);
        return false;
    }

    g_render_state = res;
    return true;
}

/*static*/ void RenderState::DestroyUniqueInstance()
{
    TM_ASSERT(g_render_state);
    delete g_render_state;
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
