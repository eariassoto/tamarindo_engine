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

#ifndef ENGINE_LIB_INPUT_INPUT_DEFS_H_
#define ENGINE_LIB_INPUT_INPUT_DEFS_H_

namespace tamarindo
{
enum class InputKeyCode : unsigned int {
    // Letters
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7,
    I = 8,
    J = 9,
    K = 10,
    L = 11,
    M = 12,
    N = 13,
    O = 14,
    P = 15,
    Q = 16,
    R = 17,
    S = 18,
    T = 19,
    U = 20,
    V = 21,
    W = 22,
    X = 23,
    Y = 24,
    Z = 25,

    KeyCount = 26
};

static inline constexpr unsigned int InputKeyCodeCount =
    static_cast<unsigned int>(InputKeyCode::KeyCount);

}  // namespace tamarindo

#endif  // ENGINE_LIB_INPUT_INPUT_DEFS_H_
