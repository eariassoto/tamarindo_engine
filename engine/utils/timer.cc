/*
 Copyright 2021-2022 Emmanuel Arias Soto

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

#include "utils/timer.h"

#include "logging/logger.h"

#include <Windows.h>

namespace tamarindo
{

Timer::Timer()
{
    if (!QueryPerformanceFrequency(&frequency_)) {
        TM_LOG_ERROR("QueryPerformanceFrequency failed.");
    }

    if (!QueryPerformanceCounter(&last_stamp_)) {
        TM_LOG_ERROR("QueryPerformanceCounter failed.");
    }
}

Timer::~Timer()
{
    TM_LOG_INFO("Total frame count: {}", frame_count_);
    TM_LOG_INFO("Total time: {}s", total_time_);
}

void Timer::StartFrame()
{
    LARGE_INTEGER new_time;
    if (!QueryPerformanceCounter(&new_time)) {
        TM_LOG_ERROR("QueryPerformanceCounter failed.");
    }

    ++frame_count_;

    const LONGLONG duration = new_time.QuadPart - last_stamp_.QuadPart;
    delta_time_ = static_cast<double>(duration) / frequency_.QuadPart;
    total_time_ += delta_time_;

    last_stamp_ = new_time;
}

}  // namespace tamarindo
