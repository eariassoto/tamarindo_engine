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

#ifndef ENGINE_LIB_RENDERING_SCENE_H_
#define ENGINE_LIB_RENDERING_SCENE_H_

#include "engine_lib/utils/timer.h"
#include "engine_lib/rendering/camera_interface.h"
#include "engine_lib/rendering/game_object.h"

#include <memory>

namespace tamarindo
{
class ShaderProgram;

class Scene
{
   public:
    void update(const Timer& timer);
    void terminate();

    bool canRender() const;
    void bindToShader(const ShaderProgram& shader_program) const;

    void setCamera(std::unique_ptr<ICamera> camera);

    inline GameObject* getGameObject() const { return m_GameObject.get(); };
    void setGameObject(std::unique_ptr<GameObject> game_object);

   private:
    std::unique_ptr<ICamera> m_Camera = nullptr;
    std::unique_ptr<GameObject> m_GameObject = nullptr;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_RENDERING_SCENE_H_
