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

class ID3D11DepthStencilView;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;
class ID3D11Texture2D;
class IDXGISwapChain;

using namespace Microsoft::WRL;

namespace tamarindo
{

class Window;
class RenderState;
class SolidColorShader;

class Renderer
{
   public:
    static std::unique_ptr<Renderer> New(RenderState* render_state,
                                         const Window& window);

    Renderer() = delete;
    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

    void Render(const SolidColorShader& shader,
                const ComPtr<ID3D11Buffer>& buffer);

   private:
    Renderer(RenderState* state_, ComPtr<IDXGISwapChain> swap_chain,
             ComPtr<ID3D11RenderTargetView> render_target_view,
             ComPtr<ID3D11DepthStencilView> depth_stencil_view);

    RenderState* state_;
    ComPtr<IDXGISwapChain> swap_chain_;
    ComPtr<ID3D11RenderTargetView> render_target_view_;
    ComPtr<ID3D11DepthStencilView> depth_stencil_view_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_RENDERER_H_
