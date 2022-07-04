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

#include "scene.h"

#include "rendering/shader_program.h"

void Scene::update(const Timer& timer)
{
    if (ICamera* camera_ptr = m_Camera.get()) {
        camera_ptr->onUpdate(timer);
    }
}

void Scene::terminate()
{
    if (m_GameObject != nullptr) {
        m_GameObject->terminate();
        m_GameObject.reset();
    }
}

bool Scene::canRender() const
{
    // TODO: Maybe return reason?
    if (m_Camera == nullptr) {
        return false;
    }
    if (m_GameObject == nullptr || !m_GameObject->hasModel()) {
        return false;
    }

    return true;
}

void Scene::bindToShader(const ShaderProgram& shader_program) const
{
    shader_program.setMat4f("viewProj", m_Camera->getViewProjectionMatrix());
}

void Scene::setCamera(std::unique_ptr<ICamera> camera)
{
    m_Camera = std::move(camera);
}

void Scene::setGameObject(std::unique_ptr<GameObject> game_object)
{
    m_GameObject = std::move(game_object);
}
