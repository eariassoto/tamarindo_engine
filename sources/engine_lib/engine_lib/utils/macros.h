// Copyright (c) 2022 Emmanuel Arias
#pragma once

namespace tamarindo
{

#ifdef DEBUG
#ifdef TM_PLATFORM_WINDOWS
#include <intrin.h>
#define TM_BREAK() __debugbreak()
#endif  // PLATFORM_WINDOWS

// TODO: test on linux
#ifdef TM_PLATFORM_LINUX
#include <signal.h>
#define TM_BREAK() raise(SIGTRAP)
#endif  // TM_PLATFORM_LINUX
#else   // DEBUG
#define TM_BREAK()
#endif  // DEBUG

}  // namespace tamarindo
