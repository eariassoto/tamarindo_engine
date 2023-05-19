/*
 Copyright 2022-2023 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_INPUT_INPUT_MANAGER_H_
#define ENGINE_LIB_INPUT_INPUT_MANAGER_H_

#include "engine_lib/input/input_defs.h"
#include "engine_lib/input/keyboard.h"

struct GLFWwindow;

namespace tamarindo
{

class InputManager
{
   public:
    bool initialize();
    void terminate();

    void startFrame();
    void finishFrame();

   private:
    Keyboard m_Keyboard;

    void keyCallback(int key, int scancode, int action, int mods);

    static void cbKeyCallback(GLFWwindow* window, int key, int scancode,
                              int action, int mods);
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_INPUT_INPUT_MANAGER_H_
