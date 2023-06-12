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
#include "rendering/vertex_buffer.h"

#include "logging/logger.h"
#include "rendering/render_state.h"

#include <d3d11.h>

namespace tamarindo
{
std::unique_ptr<VertexBuffer> VertexBuffer::New(RenderState* render_state,
                                                const void* buffer_data,
                                                size_t buffer_data_size,
                                                size_t stride)
{
    std::unique_ptr<VertexBuffer> res = std::make_unique<VertexBuffer>();

    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (unsigned int)buffer_data_size;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA buffer_resource_data;
    buffer_resource_data.pSysMem = buffer_data;
    buffer_resource_data.SysMemPitch = 0;
    buffer_resource_data.SysMemSlicePitch = 0;

    HRESULT hr = render_state->device->CreateBuffer(
        &desc, &buffer_resource_data, res->buffer.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Coult not create vertex buffer. Error: {}", hr);
        return nullptr;
    }

    return res;
}

}  // namespace tamarindo
