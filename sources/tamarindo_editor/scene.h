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

#ifndef TAMARINDO_EDITOR_SCENE_H_
#define TAMARINDO_EDITOR_SCENE_H_

#include "engine_lib/core/timer.h"
#include "engine_lib/world/camera_interface.h"
#include "engine_lib/world/game_object.h"

#include <memory>

using namespace tamarindo;

class Scene
{
   public:
    void update(const Timer& timer);
    void terminate();

    void setCamera(std::unique_ptr<ICamera> camera);
    inline ICamera* getCamera() const { return m_Camera.get(); };

    inline GameObject* getGameObject() const { return m_GameObject.get(); };
    void setGameObject(std::unique_ptr<GameObject> game_object);

   private:
    std::unique_ptr<ICamera> m_Camera = nullptr;
    std::unique_ptr<GameObject> m_GameObject = nullptr;
};

#endif  // TAMARINDO_EDITOR_SCENE_H_
