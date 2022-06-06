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

#include "rendering_manager.h"

#include "logging/logger.h"

namespace tamarindo
{

bool RenderingManager::initialize() { return true; }

void RenderingManager::terminate()
{
    for (const auto& r : m_Renderers) {
        delete r;
    }
}

void RenderingManager::addRenderer(Renderer* renderer_ptr)
{
    m_Renderers.push_back(renderer_ptr);
}

void RenderingManager::callRenderers()
{
    for (const auto& r : m_Renderers) {
        r->render();
    }
}

void RenderingManager::updateRenderers(const Timer& timer)
{
    for (const auto& r : m_Renderers) {
        r->update(timer);
    }
}

}  // namespace tamarindo
