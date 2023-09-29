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

#include "rendering/model_data.h"

#include "rendering/render_state.h"
#include "logging/logger.h"

namespace tamarindo
{

namespace
{

constexpr unsigned int MODEL_STRIDE = sizeof(float) * 5;
unsigned int BUFFER_OFFSET = 0;

}  // namespace

ModelData::ModelData(const std::vector<float>& vertex_data,
                     const std::vector<unsigned int>& index_data)
    : index_count(index_data.size())
{
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = vertex_data.size() * sizeof(float);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA buffer_resource_data;
    ZeroMemory(&buffer_resource_data, sizeof(buffer_resource_data));
    buffer_resource_data.pSysMem = vertex_data.data();
    buffer_resource_data.SysMemPitch = 0;
    buffer_resource_data.SysMemSlicePitch = 0;

    HRESULT hr = g_Device->CreateBuffer(&desc, &buffer_resource_data,
                                        vertex_buffer.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create vertex buffer. Error: {}", hr);
    }

    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(buffer_desc));
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = index_data.size() * sizeof(unsigned int);
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    ZeroMemory(&init_data, sizeof(init_data));
    init_data.pSysMem = index_data.data();

    hr = g_Device->CreateBuffer(&buffer_desc, &init_data,
                                index_buffer.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create index buffer. Error: {}", hr);
    }
}

ModelData::~ModelData() = default;

unsigned int ModelData::vertex_buffer_stride() const { return MODEL_STRIDE; }

unsigned int ModelData::vertex_buffer_offset() const { return BUFFER_OFFSET; }

unsigned int ModelData::index_buffer_offset() const { return BUFFER_OFFSET; }

}  // namespace tamarindo
