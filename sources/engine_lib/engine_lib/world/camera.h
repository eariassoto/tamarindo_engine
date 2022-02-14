// Copyright (c) 2022 Emmanuel Arias
#pragma once

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