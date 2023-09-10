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

#include <memory>

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
    class Controller
    {
       public:
        Controller() = default;
        virtual ~Controller() = default;

        virtual std::pair<XMVECTOR, XMVECTOR> GetEyeAtCameraPosition() = 0;

        virtual bool OnUpdate(const Timer& timer) = 0;
    };

    PerspectiveCamera() = delete;
    PerspectiveCamera(const PerspectiveCameraParams& params,
                      std::unique_ptr<Controller> controller);

    ~PerspectiveCamera() = default;

    void OnUpdate(const Timer& timer);

    const XMMATRIX& GetViewMat() const;
    const XMMATRIX& GetProjectionMat() const;

    unsigned int GetBufferSize();

   private:
    void MakeViewMatrix();

   private:
    float fov_angle_in_radians_;
    float aspect_ratio_;
    float z_near_;
    float z_far_;

    XMMATRIX view_matrix_ = XMMatrixIdentity();
    XMMATRIX projection_matrix_ = XMMatrixIdentity();

    std::unique_ptr<Controller> controller_ = nullptr;
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_PERSPECTIVE_CAMERA_H_
