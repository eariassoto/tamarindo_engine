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

#include "camera/spherical_camera_controller.h"

#include "camera/perspective_camera.h"
#include "utils/timer.h"
#include "utils/macros.h"
#include "input/keyboard.h"

#include <cmath>
#include <algorithm>

namespace tamarindo
{

SphericalCameraController::SphericalCameraController(
    const SphericalCameraParams& params)
    : origin_pos_(params.origin_pos),
      theta_(params.initial_theta),
      phi_(params.initial_phi),
      theta_speed_rads_per_sec_(params.theta_speed_rads_per_sec),
      phi_speed_rads_per_sec_(params.phi_speed_rads_per_sec),
      radius_size_(params.radius_size),
      pos_in_radius_(params.pos_in_radius)
{
    TM_ASSERT(phi_ >= MIN_PHI_IN_RADS);
    TM_ASSERT(phi_ <= MAX_PHI_IN_RADS);
    TM_ASSERT(pos_in_radius_ >= 0.0f);
    TM_ASSERT(pos_in_radius_ <= 1.0f);
}

bool SphericalCameraController::OnUpdate(const Timer& timer)
{
    float phi = 0;
    float theta = 0;
    int radius_zoom = 0;

    if (g_Keyboard->IsKeyPressed(InputKeyCode::W)) {
        phi += 1.0f;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::S)) {
        phi -= 1.0f;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::A)) {
        theta += 1.0f;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::D)) {
        theta -= 1.0f;
    }
    if (g_Keyboard->WasKeyPressedThisFrame(InputKeyCode::Q)) {
        --radius_zoom;
    }
    if (g_Keyboard->WasKeyPressedThisFrame(InputKeyCode::E)) {
        ++radius_zoom;
    }

    if (phi != 0 || theta != 0) {
        XMFLOAT2 vec(phi, theta);
        XMVECTOR normalized_vec = XMVector2Normalize(XMLoadFloat2(&vec));
        XMFLOAT2 res;
        XMStoreFloat2(&res, normalized_vec);

        const float df = static_cast<float>(timer.DeltaTime());
        phi_ += res.x * df * phi_speed_rads_per_sec_;
        phi_ = std::clamp(phi_, MIN_PHI_IN_RADS, MAX_PHI_IN_RADS);

        theta_ += res.y * df * theta_speed_rads_per_sec_;
        theta_ = fmod(theta_, 2 * XM_PI);
    }
    if (radius_zoom != 0) {
        pos_in_radius_ += radius_zoom * 0.15;
        pos_in_radius_ = std::clamp(pos_in_radius_, 0.15f, 1.0f);
    }

    return phi != 0 || theta != 0 || radius_zoom != 0;
}

PerspectiveCamera::Position SphericalCameraController::GetEyeAtCameraPosition()
{
    const float curr_radius_pos = pos_in_radius_ * radius_size_;
    float x = curr_radius_pos * XMScalarSin(phi_) * XMScalarCos(theta_);
    float z = curr_radius_pos * XMScalarSin(phi_) * XMScalarSin(theta_);
    float y = curr_radius_pos * XMScalarCos(phi_);
    auto eye = XMVectorSet(x, y, z, 0.0f);

    return PerspectiveCamera::Position{eye, origin_pos_};
}

}  // namespace tamarindo
