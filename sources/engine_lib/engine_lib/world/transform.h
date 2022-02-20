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

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace tamarindo
{
class Transform
{
   public:
    void initialize(const glm::vec3& position, const glm::vec3& scale);

    void setPosition(const glm::vec3& position);

    void setScale(const glm::vec3& scale);

    const glm::vec3& getPosition() const { return m_Position; }
    const glm::vec3& getScale() const { return m_Scale; }

    const glm::mat4& getTransformMatrix() const;

   private:
#ifdef DEBUG
    bool md_IsInitialized = false;
#endif  // DEBUG

    glm::vec3 m_Position;

    // TODO: implement rotation

    glm::vec3 m_Scale;

    glm::mat4 m_TransformMatrix;

    void calculateTransformMatrix();
};

}  // namespace tamarindo
