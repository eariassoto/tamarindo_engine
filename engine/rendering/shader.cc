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

#include "rendering/shader.h"

#include "logging/logger.h"
#include "rendering/render_state.h"

#include <d3d11.h>

namespace tamarindo
{

Shader::Shader(ComPtr<ID3D11VertexShader> vertex_shader,
               ComPtr<ID3D11PixelShader> pixel_shader,
               ComPtr<ID3D11InputLayout> input_layout)
    : vertex_shader_(std::move(vertex_shader)),
      pixel_shader_(std::move(pixel_shader)),
      input_layout_(std::move(input_layout))
{
}

Shader::~Shader() = default;

ID3D11VertexShader& Shader::vertex_shader() const
{
    return *vertex_shader_.Get();
}
ID3D11PixelShader& Shader::pixel_shader() const { return *pixel_shader_.Get(); }
ID3D11InputLayout& Shader::input_layout() const { return *input_layout_.Get(); }

}  // namespace tamarindo
