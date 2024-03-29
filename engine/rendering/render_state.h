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

#ifndef ENGINE_LIB_RENDERING_RENDER_STATE_H_
#define ENGINE_LIB_RENDERING_RENDER_STATE_H_

#include <wrl/client.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace tamarindo
{

namespace wrl = Microsoft::WRL;

struct RenderState {
   public:
    static RenderState* Get();

    RenderState();
    ~RenderState();

    RenderState(const RenderState& other) = delete;
    RenderState& operator=(const RenderState& other) = delete;

    bool Initialize(unsigned int width, unsigned int height);

    void Shutdown();

   private:
    bool InitializeDevice();
    bool InitializeSwapchain();
    bool InitializeRenderTargets();
    bool InitializeDepthStencilState();

    void SetRasterizerState();

   public:
    wrl::ComPtr<ID3D11Device> device;
    wrl::ComPtr<ID3D11DeviceContext> device_context;

    wrl::ComPtr<IDXGISwapChain> swap_chain;
    wrl::ComPtr<ID3D11RenderTargetView> render_target_view;
    wrl::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

   private:
    unsigned int window_height_;
    unsigned int window_width_;
};

}  // namespace tamarindo

#define g_Device ::tamarindo::RenderState::Get()->device.Get()

#endif  // ENGINE_LIB_RENDERING_RENDER_STATE_H_
