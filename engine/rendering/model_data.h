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

#ifndef ENGINE_LIB_RENDERING_MODEL_DATA_H_
#define ENGINE_LIB_RENDERING_MODEL_DATA_H_

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <vector>

namespace tamarindo
{

namespace wrl = Microsoft::WRL;

class ModelData
{
   public:
    ModelData() = delete;
    ModelData(const std::vector<float>& vertex_data,
              const std::vector<unsigned int>& index_data);
    ~ModelData();

    unsigned int vertex_buffer_stride() const;

    unsigned int vertex_buffer_offset() const;

    unsigned int index_buffer_offset() const;

    wrl::ComPtr<ID3D11Buffer> vertex_buffer;
    wrl::ComPtr<ID3D11Buffer> index_buffer;

    unsigned int index_offset = 0;
    unsigned int index_count = 0;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_MODEL_DATA_H_
