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

#include "engine_lib/rendering/renderer.h"
#include "engine_lib/rendering/window_event_handler.h"
#include "engine_lib/rendering/window.h"

#include <memory>

namespace tamarindo
{

class Application : public WindowEventHandler
{
   public:
    Application() = delete;
    Application(const HINSTANCE& hInstance, int window_show_behavior);

    ~Application() final = default;

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void Terminate();

    void Run();

   private:
    LRESULT HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam,
                                LPARAM lParam) override;

    std::unique_ptr<Window> window_;

    std::unique_ptr<Renderer> renderer_;

    int window_show_behavior_;

    bool is_running_ = true;
};

}  // namespace tamarindo

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
