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

class Timer;

struct PerspectiveCameraParams {
    float fov_angle_in_radians = DirectX::XMConvertToRadians(90.0f);
    float aspect_ratio = 0.0f;
    float z_near = 0.1f;
    float z_far = 1000.f;
};

class PerspectiveCamera
{
   public:
    struct Position {
        XMVECTOR eye_position;
        XMVECTOR look_at_position;
    };

    class Controller
    {
       public:
        Controller() = default;
        virtual ~Controller() = default;

        virtual Position GetEyeAtCameraPosition() = 0;

        virtual bool OnUpdate(const Timer& timer) = 0;
    };

    PerspectiveCamera() = delete;
    PerspectiveCamera(const PerspectiveCameraParams& params);

    ~PerspectiveCamera() = default;

    void SetController(Controller* controller);

    bool OnUpdate(const Timer& timer);

    const XMMATRIX& GetViewProjMat() const;

   private:
    void ResetMatrices(bool update_view, bool update_proj);

   private:
    float fov_angle_in_radians_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;

    XMMATRIX view_matrix_ = XMMatrixIdentity();
    XMMATRIX projection_matrix_ = XMMatrixIdentity();
    XMMATRIX view_proj_matrix_ = XMMatrixIdentity();

    Controller* controller_ = nullptr;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_PERSPECTIVE_CAMERA_H_
