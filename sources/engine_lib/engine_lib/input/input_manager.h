// Copyright (c) 2022 Emmanuel Arias
#pragma once
#include "input/input_defs.h"

#include <array>

struct GLFWwindow;

namespace tamarindo
{
class Keyboard
{
   public:
    bool initialize();

    void keyCallback(int key, int scancode, int action, int mods);

    void resetFrameKeyEvents();

    bool isKeyPressed(InputKeyCode keyCode) const;
    bool wasKeyPressedThisFrame(InputKeyCode keyCode);
    bool wasKeyReleasedThisFrame(InputKeyCode keyCode);

   private:
    std::array<bool, InputKeyCodeCount> m_KeyStatus;
    std::array<bool, InputKeyCodeCount> m_KeyPressedDuringFrame;
    std::array<bool, InputKeyCodeCount> m_KeyReleasedDuringFrame;

   private:
    static InputKeyCode convertToInputKeyCode(int glfwKeyCode);
};

class InputManager
{
   public:
    bool initialize();
    void terminate();

    void startFrame();
    void finishFrame();

    static Keyboard* getKeyboard();

   private:
    Keyboard m_Keyboard;

    void keyCallback(int key, int scancode, int action, int mods);

    static void cbKeyCallback(GLFWwindow* window, int key, int scancode,
                              int action, int mods);
};

#define g_Keyboard ::tamarindo::InputManager::getKeyboard()

}  // namespace tamarindo
