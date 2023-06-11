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

#ifndef ENGINE_LIB_UTILS_MACROS_H_
#define ENGINE_LIB_UTILS_MACROS_H_

#ifdef DEBUG
#include <intrin.h>
#endif

namespace tamarindo
{

#ifdef DEBUG
#define TM_BREAK() __debugbreak()
#else
#define TM_BREAK() ((void)0)
#endif

#ifdef DEBUG
#define TM_ASSERT(condition) \
    do {                     \
        if (!(condition)) {  \
            __debugbreak();  \
        }                    \
    } while (0) 
#else
#define TM_ASSERT(condition) ((void)0)
#endif

}  // namespace tamarindo

#endif  // ENGINE_LIB_UTILS_MACROS_H_
