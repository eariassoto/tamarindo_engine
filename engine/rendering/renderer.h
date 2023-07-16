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

#ifndef ENGINE_LIB_RENDERING_RENDERER_H_
#define ENGINE_LIB_RENDERING_RENDERER_H_

#include <d3d11.h>
#include <wrl/client.h>

#include <memory>

struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct IDXGISwapChain;
struct ID3D11Buffer;

using namespace Microsoft::WRL;

namespace tamarindo
{

class Window;
class Shader;
class VertexBuffer;
class IndexBuffer;

class Renderer
{
   public:
    static std::unique_ptr<Renderer> New(const Window& window);

    Renderer() = delete;
    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    void Render(const Shader& shader, const VertexBuffer& vb,
                const IndexBuffer& ib);

   private:
    Renderer(ComPtr<IDXGISwapChain> swap_chain,
             ComPtr<ID3D11RenderTargetView> render_target_view,
             ComPtr<ID3D11DepthStencilView> depth_stencil_view);

    ComPtr<IDXGISwapChain> swap_chain_;
    ComPtr<ID3D11RenderTargetView> render_target_view_;
    ComPtr<ID3D11DepthStencilView> depth_stencil_view_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_RENDERER_H_
