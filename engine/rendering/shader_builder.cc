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
#pragma comment(lib, "d3dcompiler")

#include "rendering/shader_builder.h"

#include "logging/logger.h"
#include "rendering/render_state.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace tamarindo::ShaderBuilder
{

namespace
{

constexpr D3D11_INPUT_ELEMENT_DESC POS_UV_LAYOUT[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
     D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3,
     D3D11_INPUT_PER_VERTEX_DATA, 0}};

constexpr unsigned int POS_UV_LAYOUT_SIZE = 2;
}  // namespace

std::unique_ptr<Shader> CompilePosUvShader(const std::string& source)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    wrl::ComPtr<ID3DBlob> cso;
    wrl::ComPtr<ID3DBlob> error_blob = nullptr;
    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vs",
               "vs_5_0", shader_flags, 0, cso.GetAddressOf(),
               error_blob.GetAddressOf());
    if (error_blob) {
        char* error_msg = static_cast<char*>(error_blob->GetBufferPointer());
        TM_LOG_ERROR("Error compiling vertex shader: {}", error_msg);
        return nullptr;
    }

    wrl::ComPtr<ID3D11VertexShader> vertex_shader;
    g_Device->CreateVertexShader(cso->GetBufferPointer(), cso->GetBufferSize(),
                                 0, vertex_shader.GetAddressOf());

    wrl::ComPtr<ID3D11InputLayout> input_layout;
    HRESULT res = g_Device->CreateInputLayout(
        POS_UV_LAYOUT, POS_UV_LAYOUT_SIZE, cso->GetBufferPointer(),
        cso->GetBufferSize(), input_layout.GetAddressOf());

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "ps",
               "ps_5_0", shader_flags, 0, cso.ReleaseAndGetAddressOf(),
               error_blob.ReleaseAndGetAddressOf());
    if (error_blob) {
        char* error_msg = static_cast<char*>(error_blob->GetBufferPointer());
        TM_LOG_ERROR("Error compiling pixel shader: {}", error_msg);
        return nullptr;
    }

    wrl::ComPtr<ID3D11PixelShader> pixel_shader;
    g_Device->CreatePixelShader(cso->GetBufferPointer(), cso->GetBufferSize(),
                                0, pixel_shader.GetAddressOf());

    return std::make_unique<Shader>(std::move(vertex_shader),
                                    std::move(pixel_shader),
                                    std::move(input_layout));
}

}  // namespace tamarindo::ShaderBuilder
