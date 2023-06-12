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

#ifndef ENGINE_LIB_UTILS_TIMER_H_
#define ENGINE_LIB_UTILS_TIMER_H_

#include <chrono>

namespace tamarindo
{
class Timer
{
   public:
    void startFrame();
    void endFrame();

    inline unsigned int frameCount() const { return m_FrameCount; }
    inline double deltaTime() const { return m_DeltaTime; }

   private:
    double m_DeltaTime = 0.0;
    unsigned int m_FrameCount = 0;

    std::chrono::high_resolution_clock::time_point m_CurrentFrameStart;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_UTILS_TIMER_H_
