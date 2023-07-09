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

#include <Windows.h>

namespace tamarindo
{
class Timer
{
   public:
    Timer();
    ~Timer();

    void StartFrame();

    inline double TotalTime() const { return total_time_; }
    inline double DeltaTime() const { return delta_time_; }

   private:
    LARGE_INTEGER frequency_;
    LARGE_INTEGER last_stamp_;

    unsigned long int frame_count_ = 0;
    double delta_time_ = 0.0;
    double total_time_ = 0.0;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_UTILS_TIMER_H_
