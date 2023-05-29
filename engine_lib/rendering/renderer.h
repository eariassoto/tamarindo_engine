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

#include <memory>

class ID3D11Device;
class ID3D11DeviceContext;

namespace tamarindo
{

class Renderer
{
   public:
    static std::unique_ptr<Renderer> CreateRenderer();

    Renderer() = delete;
    Renderer(ID3D11Device* device, ID3D11DeviceContext* device_context);
    ~Renderer();

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;

   private:
    ID3D11Device* device_;
    ID3D11DeviceContext* device_context_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_RENDERER_H_
