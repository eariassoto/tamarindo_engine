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
#include "engine_lib/utils/macros.h"

#include <d3d11.h>

namespace tamarindo
{

/*static*/ std::unique_ptr<Renderer> Renderer::CreateRenderer()
{
    D3D_FEATURE_LEVEL feature_level;
    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    HRESULT hr = D3D11CreateDevice(nullptr,                   // pAdapter
                                   D3D_DRIVER_TYPE_HARDWARE,  // DriverType
                                   nullptr,                   // Software
                                   0,                         // Flags
                                   nullptr,                   // pFeatureLevels
                                   0,                         // FeatureLevels
                                   D3D11_SDK_VERSION,         // SDKVersion
                                   &device,                   // ppDevice
                                   &feature_level,            // pFeatureLevel
                                   &device_context  // ppImmediateContext
    );
    if (FAILED(hr)) {
        TM_LOG_ERROR("D3D11CreateDevice failed.");
        return nullptr;
    }
    if (feature_level != D3D_FEATURE_LEVEL_11_0) {
        TM_LOG_ERROR("Direct3D feature level 11 unsupported.");
        return nullptr;
    }

    TM_LOG_INFO("DirectX 11 context created.");

    UINT multisample_quality;
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
                                                 &multisample_quality);
    
    TM_ASSERT(multisample_quality > 0);


    return std::make_unique<Renderer>(device, device_context);
}

Renderer::Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context)
    : device_(device), device_context_(device_context_)
{
}

Renderer::~Renderer() = default;

}  // namespace tamarindo
