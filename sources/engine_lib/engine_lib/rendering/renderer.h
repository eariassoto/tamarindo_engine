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

#ifndef ENGINE_LIB_RENDERER_H_
#define ENGINE_LIB_RENDERER_H_

namespace tamarindo
{
class Timer;

class Renderer
{
   public:
    virtual ~Renderer() = default;

    virtual bool initialize() = 0;
    virtual void terminate() = 0;

    virtual void render() = 0;
    virtual void update(const Timer& timer) = 0;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERER_H_
