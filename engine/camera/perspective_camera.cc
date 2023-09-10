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
    MakeViewMatrix();

    projection_matrix_ = XMMatrixPerspectiveFovLH(params.fov_angle_in_radians,
                                                  params.aspect_ratio,
                                                  params.z_near, params.z_far);
}

void PerspectiveCamera::OnUpdate(const Timer& timer)
{
    if (controller_->OnUpdate(timer)) {
        MakeViewMatrix();
    }
}

const XMMATRIX& PerspectiveCamera::GetViewMat() const { return view_matrix_; }

const XMMATRIX& PerspectiveCamera::GetProjectionMat() const
{
    return projection_matrix_;
}

unsigned int PerspectiveCamera::GetBufferSize() { return sizeof(view_matrix_); }

void PerspectiveCamera::MakeViewMatrix()
{
    static const XMVECTOR UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const auto& [eye, at] = controller_->GetEyeAtCameraPosition();
    view_matrix_ = XMMatrixLookAtLH(eye, at, UP);
}

}  // namespace tamarindo
