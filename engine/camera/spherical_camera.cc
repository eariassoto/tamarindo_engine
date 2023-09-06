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

#include "camera/spherical_camera.h"

#include "camera/perspective_camera.h"
#include "utils/timer.h"
#include "input/keyboard.h"

#include <algorithm>

namespace tamarindo
{

SphericalCameraController::SphericalCameraController(
    const SphericalCameraParams& params)
    : params_(params)
{
    pos_in_radius_ = params.radius;

    float x = pos_in_radius_ * XMScalarSin(phi_) * XMScalarCos(theta_);
    float z = pos_in_radius_ * XMScalarSin(phi_) * XMScalarSin(theta_);
    float y = pos_in_radius_ * XMScalarCos(phi_);

    PerspectiveCameraParams perspective_params;
    perspective_params.eye = XMVectorSet(x, y, z, 0.0f);
    perspective_params.at = XMVectorZero();
    perspective_params.aspect_ratio = params_.aspect_ratio;
    camera_ = PerspectiveCamera(perspective_params);
}

void SphericalCameraController::OnUpdate(const Timer& timer)
{
    bool needs_update = false;
    // phi, theta, radius
    float phi = 0.f;
    float theta = 0.f;

    if (g_Keyboard->IsKeyPressed(InputKeyCode::W)) {
        phi += 1.0f;
        needs_update = true;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::S)) {
        phi -= 1.0f;
        needs_update = true;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::A)) {
        theta += 1.0f;
        needs_update = true;
    }
    if (g_Keyboard->IsKeyPressed(InputKeyCode::D)) {
        theta -= 1.0f;
        needs_update = true;
    }

    if (phi != 0.0f || theta != 0.0f) {
        XMFLOAT2 vec(phi, theta);
        XMVECTOR normalized_vec = XMVector2Normalize(XMLoadFloat2(&vec));

        XMFLOAT2 res;
        XMStoreFloat2(&res, normalized_vec);

        float df = static_cast<float>(timer.DeltaTime());
        float phi_movement = res.x * df * XM_PI;
        float theta_movement = res.y * df * XM_PI;

        phi_ =
            std::clamp(phi_ + phi_movement, MIN_PHI_IN_RADS, MAX_PHI_IN_RADS);
        theta_ += theta_movement;

        float x = pos_in_radius_ * XMScalarSin(phi_) * XMScalarCos(theta_);
        float z = pos_in_radius_ * XMScalarSin(phi_) * XMScalarSin(theta_);
        float y = pos_in_radius_ * XMScalarCos(phi_);

        PerspectiveCameraParams perspective_params;
        perspective_params.eye = XMVectorSet(x, y, z, 0.0f);
        perspective_params.at = XMVectorZero();
        perspective_params.aspect_ratio = params_.aspect_ratio;
        camera_ = PerspectiveCamera(perspective_params);
    }
}

const XMMATRIX& SphericalCameraController::GetViewMat() const
{
    return camera_.GetViewMat();
}

const XMMATRIX& SphericalCameraController::GetProjectionMat() const
{
    return camera_.GetProjectionMat();
}

}  // namespace tamarindo
