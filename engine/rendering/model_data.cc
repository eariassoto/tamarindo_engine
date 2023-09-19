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

#include "rendering/buffers.h"

#include "rendering/render_state.h"
#include "logging/logger.h"

namespace tamarindo
{

namespace
{

constexpr unsigned int MODEL_STRIDE = sizeof(float) * 5;
unsigned int BUFFER_OFFSET = 0;

}  // namespace

MatrixConstantBuffer::MatrixConstantBuffer()
{
    D3D11_BUFFER_DESC camera_buf_desc;
    camera_buf_desc.Usage = D3D11_USAGE_DYNAMIC;
    camera_buf_desc.ByteWidth = sizeof(DirectX::XMMATRIX);
    camera_buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    camera_buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    camera_buf_desc.MiscFlags = 0;
    camera_buf_desc.StructureByteStride = 0;

    g_Device->CreateBuffer(&camera_buf_desc, nullptr, buffer_.GetAddressOf());
}

MatrixConstantBuffer::~MatrixConstantBuffer() = default;

ID3D11Buffer** MatrixConstantBuffer::Buffer() { return buffer_.GetAddressOf(); }

void MatrixConstantBuffer::UpdateData(const DirectX::XMMATRIX& matrix)
{
    D3D11_MAPPED_SUBRESOURCE mapped_res;
    g_DeviceContext->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
                         &mapped_res);
    DirectX::XMMATRIX* data_ptr =
        static_cast<DirectX::XMMATRIX*>(mapped_res.pData);
    *data_ptr = matrix;
}

ModelData::ModelData(const std::vector<float> vertex_data,
                     const std::vector<unsigned int> index_data)
    : index_count_(index_data.size())
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
                                        vertex_buffer_.GetAddressOf());
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
                                index_buffer_.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create index buffer. Error: {}", hr);
    }
}

ModelData::~ModelData() = default;

void ModelData::Bind() const
{
    g_DeviceContext->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(),
                                        &MODEL_STRIDE, &BUFFER_OFFSET);

    g_DeviceContext->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT,
                                      BUFFER_OFFSET);
}

}  // namespace tamarindo
