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
#include "rendering/model.h"

#include "logging/logger.h"
#include "rendering/render_state.h"

#include <d3d11.h>

namespace tamarindo
{

namespace
{

constexpr float TRIANGLE_VB[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // 1
    0.0f,  1.0f,  0.0f, 0.5f, 1.0f,  // 2
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f   // 3
};

constexpr unsigned int TRIANGLE_IB[] = {0, 1, 2};
constexpr unsigned int TRIANGLE_IB_SIZE = sizeof(TRIANGLE_IB);
constexpr unsigned int TRIANGLE_IB_COUNT = 3;

constexpr unsigned int TRIANGLE_VB_SIZE = sizeof(TRIANGLE_VB);

constexpr unsigned int MODEL_STRIDE = sizeof(float) * 5;

}  // namespace

std::unique_ptr<Model> Model::NewTriangleModel()
{
    std::unique_ptr<Model> res = std::make_unique<Model>();

    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (unsigned int)TRIANGLE_VB_SIZE;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA buffer_resource_data;
    buffer_resource_data.pSysMem = TRIANGLE_VB;
    buffer_resource_data.SysMemPitch = 0;
    buffer_resource_data.SysMemSlicePitch = 0;

    HRESULT hr = g_Device->CreateBuffer(&desc, &buffer_resource_data,
                                        res->vertex_buffer_.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create vertex buffer. Error: {}", hr);
        return nullptr;
    }

    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(buffer_desc));
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = TRIANGLE_IB_SIZE;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    ZeroMemory(&init_data, sizeof(init_data));
    init_data.pSysMem = TRIANGLE_IB;

    hr = g_Device->CreateBuffer(&buffer_desc, &init_data,
                                res->index_buffer_.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create index buffer. Error: {}", hr);
        return nullptr;
    }

    return res;
}

void Model::BindAndDraw() const
{
    g_DeviceContext->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(),
                                        &MODEL_STRIDE, &vertex_buffer_offset_);

    g_DeviceContext->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT,
                                      0);

    g_DeviceContext->DrawIndexed(TRIANGLE_IB_COUNT, 0, 0);
}

Model::Model() = default;

Model::~Model() = default;

}  // namespace tamarindo
