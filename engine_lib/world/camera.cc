/*
 Copyright 2022-2023 Emmanuel Arias Soto

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

#include "engine_lib/world/camera.h"

#include "engine_lib/core/timer.h"
#include "engine_lib/core/timer.h"
#include "engine_lib/input/input_manager.h"
#include "engine_lib/logging/logger.h"
#include "engine_lib/utils/macros.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace tamarindo
{
OrthographicCamera::OrthographicCamera(const glm::vec3& initial_position,
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

    if (!input_is_valid) {
        TM_LOG_ERROR("Projection values are not correct");
        TM_BREAK();
    }
#endif  // DEBUG
    m_Position = initial_position;
    m_ProjectionBounds = {left, right, bottom, top, z_near, z_far};
    const bool recalculate_viewproj = true;
    calculateViewMatrix(!recalculate_viewproj);
    calculateProjectionMatrix(recalculate_viewproj);
    TM_LOG_INFO("Left: {}, Right: {}, Bottom: {}, Top: {}, Near: {}, Far: {}",
                initial_position[0], initial_position[1], initial_position[2],
                left, right, bottom, top, z_near, z_far);
}

void OrthographicCamera::calculateViewMatrix(bool recalculate_viewproj)
{
    glm::mat4 translation_transform =
        glm::translate(glm::mat4(1.0f), m_Position);
    m_ViewMatrix = glm::inverse(translation_transform);

    if (recalculate_viewproj) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}

void OrthographicCamera::calculateProjectionMatrix(bool recalculate_viewproj)
{
    // TODO: Add zoom factor
    m_ProjectionMatrix = glm::orthoLH(
        m_ProjectionBounds[0], m_ProjectionBounds[1], m_ProjectionBounds[2],
        m_ProjectionBounds[3], m_ProjectionBounds[4], m_ProjectionBounds[5]);

    if (recalculate_viewproj) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}

void OrthographicCamera::setPosition(const glm::vec3& position)
{
    m_Position = position;

    const bool recalculate_viewproj = true;
    calculateViewMatrix(recalculate_viewproj);
}

PerspectiveCamera::PerspectiveCamera(float fov_angle_in_radians,
                                     float aspect_ratio, float z_near,
                                     float z_far)
    : m_FovAngleInRadians{fov_angle_in_radians},
      m_AspectRatio{aspect_ratio},
      m_NearZ{z_near},
      m_FarZ{z_far}
{
    m_ProjectionMatrix = glm::perspectiveLH_ZO(m_FovAngleInRadians,
                                               m_AspectRatio, m_NearZ, m_FarZ);
    recalculateViewProjectionMatrix();
}

void PerspectiveCamera::setPosition(glm::vec3 newPosition)
{
    m_Position = newPosition;
    recalculateViewProjectionMatrix();
}

void PerspectiveCamera::setPositionAndTarget(glm::vec3 newPosition,
                                             glm::vec3 newTarget)
{
    m_Position = newPosition;
    m_Target = newTarget;
    recalculateViewProjectionMatrix();
}

void PerspectiveCamera::recalculateViewProjectionMatrix()
{
    m_ViewMatrix =
        glm::lookAtLH(m_Position, m_Target, glm::vec3(0.0f, 1.0f, 0.0f));

    // Because GLM has column vector operations, we multiply P * V
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

SphericalCamera::SphericalCamera(const SphericalCameraParams& params)
    : PerspectiveCamera(params.FovAngleInRad, params.AspectRatio,
                        params.ZNear, params.ZFar),
      m_Params(params)
{
    m_RadiusPos = params.SphereRadius;

    float x = m_RadiusPos * sinf(m_PointPhi) * cosf(m_PointTheta);
    float z = m_RadiusPos * sinf(m_PointPhi) * sinf(m_PointTheta);
    float y = m_RadiusPos * cosf(m_PointPhi);

    setPositionAndTarget(glm::vec3(x, y, z), glm::vec3(0.0f));
}

void SphericalCamera::onUpdate(const Timer& timer)
{
    /*
    Keyboard* keyboard = g_Keyboard;

    // phi, theta, radius
    glm::vec3 movement(0.0f);

    if (keyboard->isKeyPressed(InputKeyCode::W)) {
        movement[0] += 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::S)) {
        movement[0] -= 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::A)) {
        movement[1] += 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::D)) {
        movement[1] -= 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::Q)) {
        movement[2] += 1.0f;
    }
    if (keyboard->isKeyPressed(InputKeyCode::E)) {
        movement[2] -= 1.0f;
    }

    if (movement != glm::vec3(0.0f)) {
        auto mov_norm = glm::normalize(movement);

        float df = static_cast<float>(timer.deltaTime());
        float phi_movement = mov_norm[0] * df * m_Params.SpeedRadsPerSec;
        float theta_movement = mov_norm[1] * df * m_Params.SpeedRadsPerSec;
        float radius_movement =
            mov_norm[2] * df * m_Params.MoveRadiusUnitsPerSecond;

        m_PointPhi = glm::clamp(m_PointPhi + phi_movement, m_PhiMinInRad,
                                m_PhiMaxInRad);
        m_PointTheta += theta_movement;
        m_RadiusPos = glm::clamp(m_RadiusPos + radius_movement, 0.0f,
                                 m_Params.SphereRadius);
    }

    float x = m_RadiusPos * sinf(m_PointPhi) * cosf(m_PointTheta);
    float z = m_RadiusPos * sinf(m_PointPhi) * sinf(m_PointTheta);
    float y = m_RadiusPos * cosf(m_PointPhi);

    setPosition(glm::vec3(x, y, z));
    */
}

}  // namespace tamarindo
