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

#include "camera/perspective_camera.h"

#include "utils/macros.h"
#include "logging/logger.h"

#include <DirectXMathMatrix.inl>

namespace
{

const XMVECTOR UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

}

namespace tamarindo
{

PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraParams& params,
                                     std::unique_ptr<Controller> controller)
    : fov_angle_in_radians_(params.fov_angle_in_radians),
      aspect_ratio_(params.aspect_ratio),
      z_near_(params.z_near),
      z_far_(params.z_far)
{
    controller_.swap(controller);
    ResetMatrices(/*update_view=*/true, /*update_proj=*/true);
}

void PerspectiveCamera::OnUpdate(const Timer& timer)
{
    if (controller_->OnUpdate(timer)) {
        ResetMatrices(/*update_view=*/true, /*update_proj=*/false);
    }
}

const XMMATRIX& PerspectiveCamera::GetViewProjMat() const
{
    return view_proj_matrix_;
}

unsigned int PerspectiveCamera::GetBufferSize() { return sizeof(view_matrix_); }

void PerspectiveCamera::ResetMatrices(bool update_view, bool update_proj)
{
    TM_ASSERT(update_view || update_proj);
    if (update_view) {
        const auto& [eye, at] = controller_->GetEyeAtCameraPosition();
        view_matrix_ = XMMatrixLookAtLH(eye, at, UP);
    }
    if (update_proj) {
        projection_matrix_ = XMMatrixPerspectiveFovLH(
            fov_angle_in_radians_, aspect_ratio_, z_near_, z_far_);
    }
    view_proj_matrix_ = view_matrix_ * projection_matrix_;
}

}  // namespace tamarindo
