/*
 Copyright 2022 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_INPUT_INPUT_DEFS_H_
#define ENGINE_LIB_INPUT_INPUT_DEFS_H_

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

#endif  // ENGINE_LIB_INPUT_INPUT_DEFS_H_
