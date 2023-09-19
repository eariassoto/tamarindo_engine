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

#include "rendering/matrix_constant_buffer.h"

#include "rendering/render_state.h"

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

}  // namespace tamarindo
