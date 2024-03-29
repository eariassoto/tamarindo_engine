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
#include "transform.h"

Transform::Transform() : matrix_(DirectX::XMMatrixIdentity())
{
    matrix_ = DirectX::XMMatrixRotationY(rot_y_) *
              DirectX::XMMatrixScaling(scale_, scale_, scale_) *
              DirectX::XMMatrixTranslation(0.0f, pos_y_, 0.0f);
}

Transform::~Transform() = default;

void Transform::SetScale(float scale)
{
    if (scale == scale_) {
        return;
    }
    scale_ = scale;
    matrix_ = DirectX::XMMatrixRotationY(rot_y_) *
              DirectX::XMMatrixScaling(scale_, scale_, scale_) *
              DirectX::XMMatrixTranslation(0.0f, pos_y_, 0.0f);
}

void Transform::AddRotationY(float rot)
{
    rot_y_ += rot;
    matrix_ = DirectX::XMMatrixRotationY(rot_y_) *
              DirectX::XMMatrixScaling(scale_, scale_, scale_) *
              DirectX::XMMatrixTranslation(0.0f, pos_y_, 0.0f);
}

void Transform::SetPosY(float pos)
{
    pos_y_ = pos;
    matrix_ = DirectX::XMMatrixRotationY(rot_y_) *
              DirectX::XMMatrixScaling(scale_, scale_, scale_) *
              DirectX::XMMatrixTranslation(0.0f, pos_y_, 0.0f);
}
