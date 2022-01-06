// Copyright(c) 2022 Emmanuel Arias
#include "camera.h"

#include "logging/logger.h"
#include "utils/macros.h"

#include "glm/gtc/matrix_transform.hpp"

namespace tamarindo
{
bool OrthographicCamera::initialize(const glm::vec3& initial_position,
                                    float left, float right, float bottom,
                                    float top, float z_near, float z_far)
{
#ifdef DEBUG
    // We are using the left-hand coordinate system
    // Positive directions: X to the right, Y to the top, Z points towards the
    // far
    bool input_is_valid = true;
    input_is_valid &= (left < right);
    input_is_valid &= (bottom < top);
    input_is_valid &= (z_near < z_far);
    md_IsInitialized = input_is_valid;

    if (!input_is_valid) {
        TM_LOG_ERROR("Projection values are not correct");
        TM_BREAK();
        return false;
    }
#endif  // DEBUG

    m_Position = initial_position;
    m_ProjectionBounds = {left, right, bottom, top, z_near, z_far};

    const bool should_recalculate_viewproj_matrix = true;
    calculateViewMatrix(!should_recalculate_viewproj_matrix);
    calculateProjectionMatrix(should_recalculate_viewproj_matrix);

    TM_LOG_INFO(
        "Ortographic Camera initialized. Position: ({}, {}, {}). Bounding box: "
        "Left: {}, Right: {}, Bottom: {}, Top: {}, Near: {}, Far: {}",
        initial_position[0], initial_position[1], initial_position[2], left,
        right, bottom, top, z_near, z_far);
    return true;
}

const glm::mat4& OrthographicCamera::getViewProjectionMatrix() const
{
#ifdef DEBUG
    if (!md_IsInitialized) {
        TM_BREAK();
    }
#endif  // DEBUG
    return m_ViewProjectionMatrix;
}

void OrthographicCamera::calculateViewMatrix(
    bool should_recalculate_viewproj_matrix)
{
    glm::mat4 translation_transform =
        glm::translate(glm::mat4(1.0f), m_Position);
    m_ViewMatrix = glm::inverse(translation_transform);

    if (should_recalculate_viewproj_matrix) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}

void OrthographicCamera::calculateProjectionMatrix(
    bool should_recalculate_viewproj_matrix)
{
    // TODO: Add zoom factor
    m_ProjectionMatrix = glm::orthoLH(
        m_ProjectionBounds[0], m_ProjectionBounds[1], m_ProjectionBounds[2],
        m_ProjectionBounds[3], m_ProjectionBounds[4], m_ProjectionBounds[5]);

    if (should_recalculate_viewproj_matrix) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}

void OrthographicCamera::setPosition(const glm::vec3& position)
{
    m_Position = position;

    const bool should_recalculate_viewproj_matrix = true;
    calculateViewMatrix(should_recalculate_viewproj_matrix);
}

}  // namespace tamarindo
