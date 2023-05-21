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

#include "engine_lib/input/input_manager.h"

#include "engine_lib/logging/logger.h"

#include "GLFW/glfw3.h"

namespace tamarindo
{

InputManager::InputManager(GLFWwindow* window) : m_Window(window) {}

InputManager::~InputManager() = default;

void InputManager::startFrame() {}

void InputManager::finishFrame() { m_Keyboard.resetFrameKeyEvents(); }

void InputManager::keyCallback(int key, int scancode, int action, int mods)
{
    m_Keyboard.keyCallback(key, scancode, action, mods);
}

}  // namespace tamarindo
