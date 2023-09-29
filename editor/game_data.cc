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

#include "game_data.h"

#include <array>

namespace GameData
{

namespace
{

constexpr std::array<float, 120> CUBE_VB{{
    // Front face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 2
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 3

    // Right face
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 4
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 5
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 6
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 7

    // Back face
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 8
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 9
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 10
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 11

    // Left face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 12
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 13
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 14
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    // 15

    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // 16
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   // 17
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 18
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // 19

    // Bottom face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 20
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 21
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 22
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f  // 23
}};

constexpr std::array<unsigned int, 36> CUBE_IB{{
    0,  1,  2,  2,  3,  0,   // Front face
    4,  5,  6,  6,  7,  4,   // Right face
    8,  9,  10, 10, 11, 8,   // Back face
    12, 13, 14, 14, 15, 12,  // Left face
    16, 17, 18, 18, 19, 16,  // Top face
    20, 21, 22, 22, 23, 20   // Bottom face
}};
}  // namespace

WindowData GetWindowData()
{
    return WindowData{/*.width =*/800,
                      /*.heigth =*/600,
                      /*.aspect_ratio=*/800.f / 600.f};
}

ModelData GetCubeModel()
{
    ModelData m;
    m.vertex_buffer_data = std::vector<float>(CUBE_VB.begin(), CUBE_VB.end());
    m.index_buffer_data =
        std::vector<unsigned int>(CUBE_IB.begin(), CUBE_IB.end());
    m.submodels.emplace_back(
        ModelData::SubModel{/*.vertex_offset =*/0,
                            /*.index_offset =*/0,
                            /*.index_count =*/CUBE_IB.size()});
    return m;
}

}  // namespace GameData
