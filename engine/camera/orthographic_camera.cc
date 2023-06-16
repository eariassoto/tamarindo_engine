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

#include "camera/orthographic_camera.h"

#include "utils/macros.h"
#include "logging/logger.h"

#include <DirectXMathMatrix.inl>

namespace tamarindo
{
OrthographicCamera::OrthographicCamera(const XMVECTOR& initial_position,
                                       float width, float height, float near_z,
                                       float far_z)
{
#ifdef _DEBUG
    // We are using the left-hand coordinate system
    // Positive directions: X to the right, Y to the top, Z points towards the
    // far
    bool input_is_valid = true;
    input_is_valid &= width > 0;
    input_is_valid &= height > 0;
    input_is_valid &= near_z < far_z;
    TM_ASSERT(input_is_valid);
#endif  // DEBUG
    const bool recalculate_viewproj = true;
    XMMATRIX view_mat = XMMatrixTranslationFromVector(-initial_position);
    XMMATRIX projection_mat =
        XMMatrixOrthographicLH(width, height, near_z, far_z);
    view_projection_matrix_ = view_mat * projection_mat;
}

const XMMATRIX& OrthographicCamera::GetViewProjectionMat() const
{
    return view_projection_matrix_;
}

unsigned int OrthographicCamera::GetBufferSize()
{
    return sizeof(view_projection_matrix_);
}

/*
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
    : PerspectiveCamera(params.FovAngleInRad, params.AspectRatio, params.ZNear,
                        params.ZFar),
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
}
*/

}  // namespace tamarindo
