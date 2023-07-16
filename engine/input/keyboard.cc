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

#include "input/keyboard.h"

#include <windows.h>
#include <winuser.h>

namespace tamarindo
{

namespace
{

InputKeyCode ToInputKeyCode(int key_code)
{
    if (key_code == 'A') {
        return InputKeyCode::A;
    } else if (key_code == 'B') {
        return InputKeyCode::B;
    } else if (key_code == 'C') {
        return InputKeyCode::C;
    } else if (key_code == 'D') {
        return InputKeyCode::D;
    } else if (key_code == 'E') {
        return InputKeyCode::E;
    } else if (key_code == 'F') {
        return InputKeyCode::F;
    } else if (key_code == 'G') {
        return InputKeyCode::G;
    } else if (key_code == 'H') {
        return InputKeyCode::H;
    } else if (key_code == 'I') {
        return InputKeyCode::I;
    } else if (key_code == 'J') {
        return InputKeyCode::J;
    } else if (key_code == 'K') {
        return InputKeyCode::K;
    } else if (key_code == 'L') {
        return InputKeyCode::L;
    } else if (key_code == 'M') {
        return InputKeyCode::M;
    } else if (key_code == 'N') {
        return InputKeyCode::N;
    } else if (key_code == 'O') {
        return InputKeyCode::O;
    } else if (key_code == 'P') {
        return InputKeyCode::P;
    } else if (key_code == 'Q') {
        return InputKeyCode::Q;
    } else if (key_code == 'R') {
        return InputKeyCode::R;
    } else if (key_code == 'S') {
        return InputKeyCode::S;
    } else if (key_code == 'T') {
        return InputKeyCode::T;
    } else if (key_code == 'U') {
        return InputKeyCode::U;
    } else if (key_code == 'V') {
        return InputKeyCode::V;
    } else if (key_code == 'W') {
        return InputKeyCode::W;
    } else if (key_code == 'X') {
        return InputKeyCode::X;
    } else if (key_code == 'Y') {
        return InputKeyCode::Y;
    } else if (key_code == 'Z') {
        return InputKeyCode::Z;
    }

    return InputKeyCode::KeyCount;
}

}  // namespace

Keyboard::Keyboard()
{
    m_KeyStatus.fill(false);
    m_KeyPressedDuringFrame.fill(false);
    m_KeyReleasedDuringFrame.fill(false);
}

Keyboard::~Keyboard() = default;

void Keyboard::ResetFrameKeyEvents()
{
    m_KeyPressedDuringFrame.fill(false);
    m_KeyReleasedDuringFrame.fill(false);
}

void Keyboard::KeyCallback(int key, bool is_down)
{
    const InputKeyCode key_code = ToInputKeyCode(key);
    if (key_code == InputKeyCode::KeyCount) {
        return;
    }
    const int index_in_array = static_cast<int>(key_code);
    if (is_down && !m_KeyStatus[index_in_array]) {
        m_KeyPressedDuringFrame[index_in_array] = true;
    }
    if (!is_down && m_KeyStatus[index_in_array]) {
        m_KeyReleasedDuringFrame[index_in_array] = true;
    }

    m_KeyStatus[index_in_array] = is_down;
}

bool Keyboard::IsKeyPressed(InputKeyCode key_code) const
{
    return m_KeyStatus[static_cast<int>(key_code)];
}

bool Keyboard::WasKeyPressedThisFrame(InputKeyCode key_code)
{
    return m_KeyPressedDuringFrame[static_cast<int>(key_code)];
}

bool Keyboard::WasKeyReleasedThisFrame(InputKeyCode key_code)
{
    return m_KeyReleasedDuringFrame[static_cast<int>(key_code)];
}

}  // namespace tamarindo
