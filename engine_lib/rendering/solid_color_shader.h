/*
 Copyright 2023 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_
#define ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_


namespace tamarindo
{
class SolidColorShader
{
   public:

    static std::unique_ptr<SolidColorShader> New();

};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_SOLID_COLOR_SHADER_H_
