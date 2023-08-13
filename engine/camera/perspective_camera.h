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

#ifndef ENGINE_LIB_CAMERA_PERSPECTIVE_CAMERA_H_
#define ENGINE_LIB_CAMERA_PERSPECTIVE_CAMERA_H_

#include <DirectXMath.h>

using namespace DirectX;

namespace tamarindo
{

struct PerspectiveCameraParams {
    XMVECTOR eye = XMVectorZero();
    XMVECTOR at = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    float fov_angle_in_radians = DirectX::XMConvertToRadians(90.0f);
    float aspect_ratio = 0.0f;
    float z_near = 0.1f;
    float z_far = 1000.f;
};

class PerspectiveCamera
{
   public:
    PerspectiveCamera();
    PerspectiveCamera(const PerspectiveCameraParams& params);

    // PerspectiveCamera(const PerspectiveCamera& other) = delete;
    // PerspectiveCamera& operator=(const PerspectiveCamera& other) = delete;

    virtual ~PerspectiveCamera() = default;

    const XMMATRIX& GetViewMat() const;
    const XMMATRIX& GetProjectionMat() const;

    unsigned int GetBufferSize();

   private:
    XMMATRIX view_matrix_ = XMMatrixIdentity();
    XMMATRIX projection_matrix_ = XMMatrixIdentity();
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_PERSPECTIVE_CAMERA_H_
