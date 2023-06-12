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

#ifndef ENGINE_LIB_RENDERING_VERTEX_BUFFER_H_
#define ENGINE_LIB_RENDERING_VERTEX_BUFFER_H_

#include <wrl/client.h>

#include <memory>
#include <vector>

using namespace Microsoft::WRL;

struct ID3D11Buffer;

namespace tamarindo
{

struct RenderState;

struct VertexBuffer {
   public:
    static std::unique_ptr<VertexBuffer> New(RenderState* render_state,
                                             const void* buffer_data,
                                             size_t data_size, size_t stride);

    ComPtr<ID3D11Buffer> buffer;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_VERTEX_BUFFER_H_
