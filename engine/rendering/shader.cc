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

#include "rendering/shader.h"

#include "logging/logger.h"
#include "rendering/render_state.h"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace tamarindo
{

namespace
{

}  // namespace

std::unique_ptr<Shader> Shader::New(
    RenderState* render_state, const std::string& source,
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc)
{
    std::unique_ptr<Shader> shader = std::make_unique<Shader>();

    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shader_flags |= D3DCOMPILE_DEBUG;
#endif

    ComPtr<ID3DBlob> cso;

    ComPtr<ID3DBlob> error_blob = nullptr;
    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "vs",
               "vs_5_0", shader_flags, 0, cso.GetAddressOf(),
               error_blob.GetAddressOf());
    if (error_blob) {
        char* error_msg = static_cast<char*>(error_blob->GetBufferPointer());
        TM_LOG_ERROR("Error compiling vertex shader: {}", error_msg);
        return nullptr;
    }

    render_state->device->CreateVertexShader(
        cso->GetBufferPointer(), cso->GetBufferSize(), 0,
        shader->vertex_shader.GetAddressOf());

    HRESULT res = render_state->device->CreateInputLayout(
        input_layout_desc.data(), (unsigned int)input_layout_desc.size(),
        cso->GetBufferPointer(), cso->GetBufferSize(),
        shader->input_layout.GetAddressOf());

    D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "ps",
               "ps_5_0", shader_flags, 0, cso.ReleaseAndGetAddressOf(),
               error_blob.ReleaseAndGetAddressOf());
    if (error_blob) {
        char* error_msg = static_cast<char*>(error_blob->GetBufferPointer());
        TM_LOG_ERROR("Error compiling pixel shader: {}", error_msg);
        return nullptr;
    }

    render_state->device->CreatePixelShader(
        cso->GetBufferPointer(), cso->GetBufferSize(), 0,
        shader->pixel_shader.GetAddressOf());

    return shader;
}

}  // namespace tamarindo
