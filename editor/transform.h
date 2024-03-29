/*
 Copyright 2021-2023 Emmanuel Arias Soto

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
#ifndef TAMARINDO_EDITOR_TRANSFORM_H_
#define TAMARINDO_EDITOR_TRANSFORM_H_

#include <DirectXMath.h>

class Transform
{
   public:
    Transform();
    ~Transform();

    inline const DirectX::XMMATRIX& GetMatrix() { return matrix_; }

    void SetScale(float scale);

    void AddRotationY(float rot);

    void SetPosY(float pos);

   private:
    float scale_ = 1.0f;
    float rot_y_ = 0.0f;
    float pos_y_ = 0.0f;

    DirectX::XMMATRIX matrix_ = DirectX::XMMatrixIdentity();
};

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
