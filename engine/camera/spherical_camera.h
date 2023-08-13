/*
 Copyright 2023 Emmanuel Arias Soto

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

#ifndef ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_H_
#define ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_H_

#include "camera/perspective_camera.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace tamarindo
{
class Timer;

struct SphericalCameraParams {
    XMVECTOR position = XMVectorZero();
    float radius = 4.0f;

    float fov_angle_in_radians = DirectX::XMConvertToRadians(90.0f);
    float aspect_ratio = 0.0f;
    float z_near = 0.1f;
    float z_far = 1000.f;
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
class SphericalCamera /*: public PerspectiveCamera*/
{
   public:
    SphericalCamera(const SphericalCameraParams& params);

    void OnUpdate(const Timer& timer);

    const XMMATRIX& GetViewProjectionMat() const;

   private:
    static constexpr float MIN_PHI_IN_RADS = 0.1f;
    static constexpr float MAX_PHI_IN_RADS = XM_PI - 0.1f;

    // TODO: provide an API to have an initial thetha and phi from vector
    float theta_ = 1.5f * XM_PI;
    float phi_ = 0.3f * XM_PI;
    float pos_in_radius_ = 0.0f;

    SphericalCameraParams params_;
    PerspectiveCamera camera_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_H_
