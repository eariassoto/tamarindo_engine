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

VertexBuffer::VertexBuffer() = default;

VertexBuffer::VertexBuffer(unsigned int stride, void* data,
                           unsigned int data_byte_size)
    : stride_(stride)
{
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = data_byte_size;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA buffer_resource_data;
    ZeroMemory(&buffer_resource_data, sizeof(buffer_resource_data));
    buffer_resource_data.pSysMem = data;
    buffer_resource_data.SysMemPitch = 0;
    buffer_resource_data.SysMemSlicePitch = 0;

    HRESULT hr = g_Device->CreateBuffer(&desc, &buffer_resource_data,
                                        buffer_.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create vertex buffer. Error: {}", hr);
    }
}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::Bind() const
{
    g_DeviceContext->IASetVertexBuffers(0, 1, buffer_.GetAddressOf(), &stride_,
                                        &offset_);
}

IndexBuffer::IndexBuffer() = default;

IndexBuffer::IndexBuffer(unsigned int index_count, void* data,
                         unsigned int data_byte_size)
    : index_count_(index_count)
{
    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(buffer_desc));
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth = data_byte_size;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data;
    ZeroMemory(&init_data, sizeof(init_data));
    init_data.pSysMem = data;

    HRESULT hr = g_Device->CreateBuffer(&buffer_desc, &init_data,
                                        buffer_.GetAddressOf());
    if (FAILED(hr)) {
        TM_LOG_ERROR("Could not create index buffer. Error: {}", hr);
    }
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::Bind() const
{
    g_DeviceContext->IASetIndexBuffer(buffer_.Get(), DXGI_FORMAT_R32_UINT,
                                      offset_);
}

void IndexBuffer::Draw() const
{
    g_DeviceContext->DrawIndexed(index_count_, 0, 0);
}
}  // namespace tamarindo
