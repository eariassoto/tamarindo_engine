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

#include "engine_lib/rendering/solid_color_shader.h"

#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/render_state.h"
#include "engine_lib/rendering/vertex_input.h"

#include <fstream>
#include <string>
#include <vector>

namespace tamarindo
{

namespace
{

std::vector<char> LoadShaderFromFile(const std::string& file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return {};
    }

    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> shader_data;
    shader_data.resize(file_size);
    if (!file.read(shader_data.data(), file_size)) return {};

    return shader_data;
}

D3D11_INPUT_ELEMENT_DESC INPUT_DESC[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
     offsetof(VertexInput, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
};

}  // namespace

std::unique_ptr<SolidColorShader> SolidColorShader::New(
    RenderState* render_state)
{
    std::vector<char> vs_bytecode = LoadShaderFromFile("../solid_color_vs.cso");
    ID3D11Device& device = render_state->Device();

    ComPtr<ID3D11VertexShader> vertex_shader;
    HRESULT result =
        device.CreateVertexShader(vs_bytecode.data(), vs_bytecode.size(),
                                  nullptr, vertex_shader.GetAddressOf());
    if (FAILED(result)) {
        TM_LOG_ERROR("Could not create vertex shader.");
        return nullptr;
    }

    std::vector<char> ps_bytecode = LoadShaderFromFile("../solid_color_ps.cso");

    ComPtr<ID3D11PixelShader> pixel_shader;
    result = device.CreatePixelShader(ps_bytecode.data(), ps_bytecode.size(),
                                      nullptr, pixel_shader.GetAddressOf());
    if (FAILED(result)) {
        TM_LOG_ERROR("Could not create pixel shader.");
        return nullptr;
    }

    ComPtr<ID3D11InputLayout> input_layout;
    result = device.CreateInputLayout(INPUT_DESC, ARRAYSIZE(INPUT_DESC),
                                      vs_bytecode.data(), vs_bytecode.size(),
                                      &input_layout);

    if (FAILED(result)) {
        TM_LOG_ERROR("Could not create input layout.");
        return nullptr;
    }

    return std::unique_ptr<SolidColorShader>(
        new SolidColorShader(vertex_shader, pixel_shader, input_layout));
}

SolidColorShader::SolidColorShader(ComPtr<ID3D11VertexShader> vertex_shader,
                                   ComPtr<ID3D11PixelShader> pixel_shader,
                                   ComPtr<ID3D11InputLayout> input_layout)
    : vertex_shader_(vertex_shader),
      pixel_shader_(pixel_shader),
      input_layout_(input_layout)
{
}

}  // namespace tamarindo
