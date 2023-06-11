/*
 Copyright 2021-2023 Emmanuel Arias Soto

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

#ifndef TAMARINDO_EDITOR_APPLICATION_H_
#define TAMARINDO_EDITOR_APPLICATION_H_
#pragma comment(lib, "d3d11")

#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

struct IDXGISwapChain;

namespace tamarindo
{

class Application
{
   public:
    Application() = delete;
    explicit Application(int window_show_behavior);

    ~Application() = default;

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void Run();

   private:
    ComPtr<IDXGISwapChain> swap_chain_;

    int window_show_behavior_;
};

}  // namespace tamarindo

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
