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

#ifndef ENGINE_LIB_CAMERA_ORTHOGRAPHIC_CAMERA_H_
#define ENGINE_LIB_CAMERA_ORTHOGRAPHIC_CAMERA_H_

#include <DirectXMath.h>

using namespace DirectX;

namespace tamarindo
{
class OrthographicCamera
{
   public:
    OrthographicCamera() = delete;
    OrthographicCamera(const XMVECTOR& initial_position, float width,
                       float height, float near_z, float far_z);

    OrthographicCamera(const OrthographicCamera& other) = delete;
    OrthographicCamera& operator=(const OrthographicCamera& other) = delete;

    const XMMATRIX& GetViewProjectionMat() const;

    unsigned int GetBufferSize();

   private:
    XMMATRIX view_projection_matrix_ = XMMatrixIdentity();
};

/*
class PerspectiveCamera : public ICamera
{
   public:
    PerspectiveCamera() = delete;
    PerspectiveCamera(float fov_angle_in_radians, float aspect_ratio,
                      float z_near, float z_far);

    virtual ~PerspectiveCamera() = default;

    void setPosition(glm::vec3 newPosition);
    void setPositionAndTarget(glm::vec3 newPosition, glm::vec3 newTarget);

    inline const glm::vec3& getPosition() const { return m_Position; };
    inline const glm::vec3& getTarget() const { return m_Target; };

    const glm::mat4& getViewProjectionMatrix() const override
    {
        return m_ViewProjectionMatrix;
    }

    virtual void onUpdate(const Timer& timer) = 0;

   private:
    void recalculateViewProjectionMatrix();

   private:
    float m_FovAngleInRadians;
    float m_AspectRatio;
    float m_NearZ;
    float m_FarZ;

    glm::vec3 m_Position;
    glm::vec3 m_Target;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0);
};

struct SphericalCameraParams {
    float FovAngleInRad;
    float AspectRatio;
    float ZNear;
    float ZFar;

    glm::vec3 SpherePosition;
    float SphereRadius;

    float MoveRadiusUnitsPerSecond;
    float SpeedRadsPerSec;
};

/// <summary>
/// This type of perspective camera uses spherical coordinates for movement.
/// The angles phi and theta and the radius of the sphere determine the
/// position of the camera.
///
/// For a visual representation consider:
///
///           z ^
///             |
///             |
///             |
///             |
///             |                          Position
///             |                     -->X
///             |        radius    --/
///             |                -/
///             |             --/
///             | Phi      --/
///             |-\     --/
///             |  -  -/
///             |  --/
///             |-/
///      target |------------------------------->
///            / ---/                           y
///           /-/
///          /     Theta
///         /
///        /
///       /
///      /
///     /
///    -
/// x v
///
/// The cartesian coordinates for this system are given by:
///
/// x = radius * sin(phi) * cos(theta)
/// y = radius * sin(phi) * sin(theta)
/// z = radius * cos(phi)
///
/// </summary>
class SphericalCamera : public PerspectiveCamera
{
   public:
    SphericalCamera(const SphericalCameraParams& params);

    void onUpdate(const Timer& timer) override;

   private:
    static constexpr float m_PhiMinInRad = 0.1f;
    static constexpr float m_PhiMaxInRad = glm::pi<float>() - 0.1f;

    // TODO: provide an API to have an initial thetha and phi from vector
    float m_PointTheta = 1.5f * glm::pi<float>();
    float m_PointPhi = 0.3f * glm::pi<float>();
    float m_RadiusPos = 0.0f;

    SphericalCameraParams m_Params;
};
*/
}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_ORTHOGRAPHIC_CAMERA_H_
