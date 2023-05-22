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

#include "engine_lib/utils/timer.h"

namespace tamarindo
{
void Timer::startFrame()
{
    m_CurrentFrameStart = std::chrono::high_resolution_clock::now();
    ++m_FrameCount;
}

void Timer::endFrame()
{
    auto current_frame_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed =
        current_frame_end - m_CurrentFrameStart;
    m_DeltaTime = elapsed.count();
}

}  // namespace tamarindo
