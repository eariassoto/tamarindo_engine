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

#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

using namespace Microsoft::WRL;

namespace tamarindo
{

class Window;

struct RenderState {
   public:
    static std::unique_ptr<RenderState> New(const Window& window);

    RenderState();
    ~RenderState();

    RenderState(const RenderState& other) = delete;
    RenderState& operator=(const RenderState& other) = delete;

    ComPtr<ID3D11Device> device;

    ComPtr<ID3D11DeviceContext> device_context;

    ComPtr<IDXGISwapChain> swap_chain;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_RENDER_STATE_H_
