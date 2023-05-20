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

#include "engine_lib/rendering/rendering_manager.h"

#include "engine_lib/logging/logger.h"

namespace tamarindo
{

RenderingManager::RenderingManager(GLFWwindow* window)
{
    m_SceneRenderer = std::make_unique<SceneRenderer>(window);
    m_ImGuiRenderer = std::make_unique<ImGuiRenderer>(window);
}

RenderingManager::~RenderingManager() = default;

void RenderingManager::terminate()
{
    m_ImGuiRenderer->terminate();
    m_SceneRenderer->terminate();
}

void RenderingManager::update(const Timer& timer)
{
    m_SceneRenderer->update(timer);
    m_ImGuiRenderer->update(timer);
}

void RenderingManager::render()
{
    m_SceneRenderer->render();
    m_ImGuiRenderer->render();
}

}  // namespace tamarindo
