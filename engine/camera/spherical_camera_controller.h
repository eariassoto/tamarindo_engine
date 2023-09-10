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

#ifndef ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_CONTROLLER_H_
#define ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_CONTROLLER_H_

#include "camera/perspective_camera.h"

#include <DirectXMath.h>

using namespace DirectX;

namespace tamarindo
{
class Timer;
class PerspectiveCamera;

struct SphericalCameraParams {
    XMVECTOR origin_pos = XMVectorZero();

    float initial_theta = 1.5f * XM_PI;
    float initial_phi = 0.3f * XM_PI;

    float theta_speed_rads_per_sec = XM_PI;
    float phi_speed_rads_per_sec = XM_PI;

    float radius_size = 2.0f;
    float pos_in_radius = 1.0f;
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
class SphericalCameraController : public PerspectiveCamera::Controller
{
   public:
    SphericalCameraController(const SphericalCameraParams& params);

    // PerspectiveCamera::Controller overrides
    bool OnUpdate(const Timer& timer) override;

    std::pair<XMVECTOR, XMVECTOR> GetEyeAtCameraPosition() override;

   private:
    static constexpr float MIN_PHI_IN_RADS = 0.1f;
    static constexpr float MAX_PHI_IN_RADS = XM_PI - 0.1f;

    XMVECTOR origin_pos_;

    float theta_;
    float phi_;

    float theta_speed_rads_per_sec_;
    float phi_speed_rads_per_sec_;

    float radius_size_;
    float pos_in_radius_;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_SPHERICAL_CAMERA_CONTROLLER_H_
