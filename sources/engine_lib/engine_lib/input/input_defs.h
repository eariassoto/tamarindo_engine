// Copyright (c) 2022 Emmanuel Arias
#pragma once

namespace tamarindo
{
enum class InputKeyCode : unsigned int {
    // Normal Letters
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    // Don't add after this
    KeyCount
};

static inline constexpr unsigned int InputKeyCodeCount =
    static_cast<unsigned int>(InputKeyCode::KeyCount);

}  // namespace tamarindo
