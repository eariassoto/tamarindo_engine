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

#ifndef ENGINE_LIB_WORLD_CAMERA_H_
#define ENGINE_LIB_WORLD_CAMERA_H_

#include "glm/glm.hpp"

#include <array>

namespace tamarindo
{
class OrthographicCamera
{
   public:
    bool initialize(const glm::vec3& initial_position, float left, float right,
                    float bottom, float top, float z_near, float z_far);

    void setPosition(const glm::vec3& position);

    const glm::vec3& getPosition() const { return m_Position; }

    const glm::mat4& getViewProjectionMatrix() const;

   private:
    void calculateViewMatrix(bool should_recalculate_viewproj_matrix);
    void calculateProjectionMatrix(bool should_recalculate_viewproj_matrix);

   private:
#ifdef DEBUG
    bool md_IsInitialized = false;
#endif  // DEBUG

    glm::vec3 m_Position;

    // Stored as: left, right, bottom, top, near, far
    std::array<float, 6> m_ProjectionBounds;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

    // This matrix is calculated every time either m_ProjectionMatrix or
    // m_ViewMatrix changes
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_WORLD_CAMERA_H_
