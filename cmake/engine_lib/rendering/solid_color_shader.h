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

#ifndef ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_
#define ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_

#include <wrl/client.h>
#include <d3d11.h>

#include <memory>

using namespace Microsoft::WRL;

class ID3D11PixelShader;
class ID3D11VertexShader;

namespace tamarindo
{

class RenderState;

class SolidColorShader
{
   public:
    static std::unique_ptr<SolidColorShader> New(RenderState* render_state);

    SolidColorShader(ComPtr<ID3D11VertexShader> vertex_shader,
                     ComPtr<ID3D11PixelShader> pixel_shader,
                     ComPtr<ID3D11InputLayout> input_layout);

    ComPtr<ID3D11VertexShader> vertex_shader_;
    ComPtr<ID3D11PixelShader> pixel_shader_;
    ComPtr<ID3D11InputLayout> input_layout_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_
