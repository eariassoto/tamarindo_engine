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

#ifndef ENGINE_LIB_RENDERING_WINDOW_H_
#define ENGINE_LIB_RENDERING_WINDOW_H_

#include <windows.h>

#include <memory>
#include <string>

namespace tamarindo
{

class WindowEventHandler;

class Window
{
   public:
    // TODO: Consider implementing a WindowBuilder
    static std::unique_ptr<Window> CreateWithClass(
        const std::string& class_name,
        WindowEventHandler* window_event_handler);

    Window() = delete;

    ~Window();

    Window(const Window& other) = delete;
    Window& operator=(const Window& other) = delete;

    void Show(int show_behavior);

    void Update();

    unsigned int Width() const;

    unsigned int Height() const;

    HWND Handle() const;

   private:
    Window(HWND window_handle, unsigned int width, unsigned int height);

    HWND window_handle_;

    unsigned int width_;
    unsigned int height_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_WINDOW_H_
