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

#ifndef TAMARINDO_EDITOR_EDITOR_H_
#define TAMARINDO_EDITOR_EDITOR_H_

#include "scene.h"

#include "engine_lib/core/application.h"
#include "engine_lib/rendering/shader_program.h"
#include "engine_lib/world/camera_interface.h"

#include <memory>

using namespace tamarindo;

class Editor : public Application
{
   private:
    std::unique_ptr<ICamera> m_Camera = nullptr;

    bool doInitialize() override;
    bool doPreInitialize() override;
    std::unique_ptr<ApplicationProperties> loadApplicationProperties() override;
    void doTerminate() override;

    void doUpdate(const Timer& timer) override;
    void doRender() override;

    std::unique_ptr<ShaderProgram> m_ShaderProgram = nullptr;

    std::unique_ptr<Scene> m_MainScene = nullptr;
};

std::unique_ptr<Application> CreateApplication();

#endif  // TAMARINDO_EDITOR_EDITOR_H_
