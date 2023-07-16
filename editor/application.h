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

#include "camera/perspective_camera.h"
#include "rendering/buffers.h"
#include "rendering/render_state.h"
#include "rendering/renderer.h"
#include "window/window.h"
#include "window/window_event_handler.h"

#include <memory>

using namespace Microsoft::WRL;

namespace tamarindo
{

class Application : public WindowEventHandler
{
   public:
    Application() = delete;
    explicit Application(int window_show_behavior);

    ~Application();

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void Run();

   private:
    bool is_running_ = true;

    std::unique_ptr<Window> window_;

    std::unique_ptr<Renderer> renderer_;

    std::unique_ptr<Shader> shader_;

    std::unique_ptr<Model> model_;

    std::unique_ptr<PerspectiveCamera> camera_;

    int window_show_behavior_;

    std::unique_ptr<MatrixConstantBuffer> mvp_cb_;

    virtual LRESULT HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam,
                                        LPARAM lParam) override;
};

}  // namespace tamarindo

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
