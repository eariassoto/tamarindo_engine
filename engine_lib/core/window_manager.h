/*
 Copyright 2021-2022 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_CORE_WINDOW_MANAGER_H_
#define ENGINE_LIB_CORE_WINDOW_MANAGER_H_

#include <array>
#include <memory>
#include <string>

// Forward declarations
struct GLFWwindow;

namespace tamarindo
{
class ApplicationProperties;

class WindowManager
{
   public:
    bool initialize(const ApplicationProperties& properties);
    void terminate();

    void processEvents();
    void swapBuffers();

    bool shouldWindowClose() const;

   private:
    GLFWwindow* m_Window = nullptr;

    static void glfwErrorCb(int error, const char* desc);
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CORE_WINDOW_MANAGER_H_
