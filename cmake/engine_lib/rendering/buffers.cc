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
#include "engine_lib/rendering/buffers.h"

#include "engine_lib/rendering/render_state.h"
#include "engine_lib/rendering/vertex_input.h"

namespace tamarindo
{

ComPtr<ID3D11Buffer> CreateVertexBuffer(RenderState* render_state,
                                        const void* vertices,
                                        size_t num_vertices)
{
    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(buffer_desc));
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = sizeof(VertexInput) * num_vertices;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    ZeroMemory(&init_data, sizeof(init_data));
    init_data.pSysMem = vertices;

    ComPtr<ID3D11Buffer> vertex_buffer;
    HRESULT result = render_state->Device().CreateBuffer(
        &buffer_desc, &init_data, vertex_buffer.GetAddressOf());
    if (FAILED(result)) {
        // Error handling
        return nullptr;
    }

    return vertex_buffer;
}

}  // namespace tamarindo
