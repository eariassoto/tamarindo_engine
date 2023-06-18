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

}  // namespace tamarindo

#endif  // ENGINE_LIB_CAMERA_ORTHOGRAPHIC_CAMERA_H_
