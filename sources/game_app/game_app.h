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

#pragma once
#include "engine_lib/core/application.h"
#include "engine_lib/core/window_manager.h"
#include "engine_lib/rendering/mesh.h"
#include "engine_lib/rendering/shader_program.h"
#include "engine_lib/world/camera.h"
#include "engine_lib/world/transform.h"

#include <memory>

using namespace tamarindo;

class GameApp : public Application
{
   public:
    GameApp();

   private:
    WindowProperties m_WindowProperties;

    OrthographicCamera m_Camera;

    Transform m_SquareMeshTransform;

    bool doInitialize() override;
    void doTerminate() override;

    void doUpdate(std::chrono::duration<double> total_time,
                  std::chrono::duration<double> delta_time) override;
    void doRender() override;

    const WindowProperties& getWindowProperties() const override
    {
        return m_WindowProperties;
    };

    ShaderProgramID m_ShaderProgram = 0;
    MeshInstanceID m_SquareMeshInstance = {};
};

std::unique_ptr<Application> CreateApplication();
