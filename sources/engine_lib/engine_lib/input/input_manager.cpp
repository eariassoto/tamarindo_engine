// Copyright (c) 2022 Emmanuel Arias
#include "input_manager.h"

#include "core/application.h"
#include "core/window_manager.h"
#include "logging/logger.h"

#include "GLFW/glfw3.h"

namespace tamarindo
{
namespace
{
InputManager *s_InputManager = nullptr;
}

#pragma region Keyboard

bool Keyboard::initialize()
{
    m_KeyStatus.fill(false);
    m_KeyPressedDuringFrame.fill(false);
    m_KeyReleasedDuringFrame.fill(false);

    return true;
}

void Keyboard::resetFrameKeyEvents()
{
    m_KeyPressedDuringFrame.fill(false);
    m_KeyReleasedDuringFrame.fill(false);
}

void Keyboard::keyCallback(int key, int scancode, int action, int mods)
{
    InputKeyCode keyCode = convertToInputKeyCode(key);
    if (keyCode == InputKeyCode::KeyCount) {
        return;
    }
    int indexInArray = static_cast<int>(keyCode);
    if (action == GLFW_PRESS) {
        if (!m_KeyStatus[indexInArray]) {
            m_KeyPressedDuringFrame[indexInArray] = true;
        }
        m_KeyStatus[indexInArray] = true;
    } else if (action == GLFW_RELEASE) {
        if (m_KeyStatus[indexInArray]) {
            m_KeyReleasedDuringFrame[indexInArray] = true;
        }
        m_KeyStatus[indexInArray] = false;
    }
}

bool Keyboard::isKeyPressed(InputKeyCode keyCode) const
{
    return m_KeyStatus[static_cast<int>(keyCode)];
}

bool Keyboard::wasKeyPressedThisFrame(InputKeyCode keyCode)
{
    return m_KeyPressedDuringFrame[static_cast<int>(keyCode)];
}

bool Keyboard::wasKeyReleasedThisFrame(InputKeyCode keyCode)
{
    return m_KeyReleasedDuringFrame[static_cast<int>(keyCode)];
}

/*static*/ InputKeyCode Keyboard::convertToInputKeyCode(int glfwKeyCode)
{
    if (glfwKeyCode == GLFW_KEY_A) {
        return InputKeyCode::A;
    } else if (glfwKeyCode == GLFW_KEY_B) {
        return InputKeyCode::B;
    } else if (glfwKeyCode == GLFW_KEY_C) {
        return InputKeyCode::C;
    } else if (glfwKeyCode == GLFW_KEY_D) {
        return InputKeyCode::D;
    } else if (glfwKeyCode == GLFW_KEY_E) {
        return InputKeyCode::E;
    } else if (glfwKeyCode == GLFW_KEY_F) {
        return InputKeyCode::F;
    } else if (glfwKeyCode == GLFW_KEY_G) {
        return InputKeyCode::G;
    } else if (glfwKeyCode == GLFW_KEY_H) {
        return InputKeyCode::H;
    } else if (glfwKeyCode == GLFW_KEY_I) {
        return InputKeyCode::I;
    } else if (glfwKeyCode == GLFW_KEY_J) {
        return InputKeyCode::J;
    } else if (glfwKeyCode == GLFW_KEY_K) {
        return InputKeyCode::K;
    } else if (glfwKeyCode == GLFW_KEY_L) {
        return InputKeyCode::L;
    } else if (glfwKeyCode == GLFW_KEY_M) {
        return InputKeyCode::M;
    } else if (glfwKeyCode == GLFW_KEY_N) {
        return InputKeyCode::N;
    } else if (glfwKeyCode == GLFW_KEY_O) {
        return InputKeyCode::O;
    } else if (glfwKeyCode == GLFW_KEY_P) {
        return InputKeyCode::P;
    } else if (glfwKeyCode == GLFW_KEY_Q) {
        return InputKeyCode::Q;
    } else if (glfwKeyCode == GLFW_KEY_R) {
        return InputKeyCode::R;
    } else if (glfwKeyCode == GLFW_KEY_S) {
        return InputKeyCode::S;
    } else if (glfwKeyCode == GLFW_KEY_T) {
        return InputKeyCode::T;
    } else if (glfwKeyCode == GLFW_KEY_U) {
        return InputKeyCode::U;
    } else if (glfwKeyCode == GLFW_KEY_V) {
        return InputKeyCode::V;
    } else if (glfwKeyCode == GLFW_KEY_W) {
        return InputKeyCode::W;
    } else if (glfwKeyCode == GLFW_KEY_X) {
        return InputKeyCode::X;
    } else if (glfwKeyCode == GLFW_KEY_Y) {
        return InputKeyCode::Y;
    } else if (glfwKeyCode == GLFW_KEY_Z) {
        return InputKeyCode::Z;
    }

    return InputKeyCode::KeyCount;
}

#pragma endregion

bool InputManager::initialize()
{
    glfwSetKeyCallback(g_Window, InputManager::cbKeyCallback);

    s_InputManager = this;

    return m_Keyboard.initialize();
}

void InputManager::terminate() {}

void InputManager::startFrame()
{
    if (glfwGetKey(g_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        g_Application->stop();
    }
}

void InputManager::finishFrame() { m_Keyboard.resetFrameKeyEvents(); }

Keyboard *InputManager::getKeyboard() { return &s_InputManager->m_Keyboard; }

void InputManager::keyCallback(int key, int scancode, int action, int mods)
{
    m_Keyboard.keyCallback(key, scancode, action, mods);
}

/*static*/ void InputManager::cbKeyCallback(GLFWwindow *window, int key,
                                            int scancode, int action, int mods)
{
    s_InputManager->keyCallback(key, scancode, action, mods);
}

}  // namespace tamarindo
