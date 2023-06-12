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

#ifndef ENGINE_LIB_RENDERING_SHADER_H_
#define ENGINE_LIB_RENDERING_SHADER_H_

#include <wrl/client.h>

#include <memory>
#include <string>
#include <vector>

using namespace Microsoft::WRL;

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace tamarindo
{

struct Shader {
   public:
    static std::unique_ptr<Shader> New(
        const std::string& source,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc);

    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11PixelShader> pixel_shader;
    ComPtr<ID3D11InputLayout> input_layout;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_SHADER_H_
