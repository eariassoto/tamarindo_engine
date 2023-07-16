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

#ifndef ENGINE_LIB_RENDERING_BUFFERS_H_
#define ENGINE_LIB_RENDERING_BUFFERS_H_

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

using namespace Microsoft::WRL;

namespace tamarindo
{

class MatrixConstantBuffer
{
   public:
    MatrixConstantBuffer();
    ~MatrixConstantBuffer();

    ID3D11Buffer** Buffer();

    void UpdateData(const DirectX::XMMATRIX& matrix);

   private:
    ComPtr<ID3D11Buffer> buffer_;
};

class VertexBuffer
{
   public:
    VertexBuffer();
    VertexBuffer(unsigned int stride, void* data, unsigned int data_byte_size);
    ~VertexBuffer();

    void Bind() const;

   private:
    ComPtr<ID3D11Buffer> buffer_;

    unsigned int stride_ = 0;
    unsigned int offset_ = 0;
};

class IndexBuffer
{
   public:
    IndexBuffer();
    IndexBuffer(unsigned int index_count, void* data,
                unsigned int data_byte_size);
    ~IndexBuffer();

    void Bind() const;
    void Draw() const;

   private:
    ComPtr<ID3D11Buffer> buffer_;

    unsigned int offset_ = 0;
    unsigned int index_count_ = 0;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_BUFFERS_H_