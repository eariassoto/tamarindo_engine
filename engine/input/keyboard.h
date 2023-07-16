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

#ifndef ENGINE_LIB_INPUT_KEYBOARD_H_
#define ENGINE_LIB_INPUT_KEYBOARD_H_

#include "input/input_defs.h"

#include <array>

namespace tamarindo
{
class Keyboard
{
   public:
    Keyboard();
    ~Keyboard();

    Keyboard(const Keyboard& other) = delete;
    Keyboard& operator=(const Keyboard& other) = delete;

    void KeyCallback(int key, bool is_down);

    void ResetFrameKeyEvents();

    bool IsKeyPressed(InputKeyCode key_code) const;
    bool WasKeyPressedThisFrame(InputKeyCode key_code);
    bool WasKeyReleasedThisFrame(InputKeyCode key_code);

   private:
    std::array<bool, InputKeyCodeCount> m_KeyStatus;
    std::array<bool, InputKeyCodeCount> m_KeyPressedDuringFrame;
    std::array<bool, InputKeyCodeCount> m_KeyReleasedDuringFrame;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_INPUT_KEYBOARD_H_
