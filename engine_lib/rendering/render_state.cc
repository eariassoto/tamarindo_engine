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

#include "engine_lib/rendering/render_state.h"

#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/window.h"
#include "engine_lib/utils/macros.h"

#include <d3d11.h>

namespace tamarindo
{

/*static*/ std::unique_ptr<RenderState> RenderState::CreateRenderState(
    const Window& window)
{
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

    HRESULT res = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                    0, nullptr, 0, D3D11_SDK_VERSION, &device,
                                    &feature_level, &device_context);

    if (FAILED(res)) {
        TM_LOG_ERROR("D3D11CreateDevice failed.");
        return nullptr;
    }

    TM_LOG_DEBUG("DirectX11 device context created.");

    return std::unique_ptr<RenderState>(new RenderState(
        device, device_context));
}

RenderState::RenderState(ID3D11Device* device, ID3D11DeviceContext* device_context)
    : device_(device),
      device_context_(device_context)
{
}

RenderState::~RenderState()
{
    device_context_->Release();
    device_->Release();
}

ID3D11Device& RenderState::Device() { return *device_; }

ID3D11DeviceContext& RenderState::DeviceContext() { return *device_context_; }

}  // namespace tamarindo
