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
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 16
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 17
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 18
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 19

    // Bottom face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 20
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 21
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 22
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f  // 23
}};

constexpr std::array<unsigned int, 36> CUBE_IB{{
    2,  1,  0,  0,  3,  2,   // Front face
    6,  5,  4,  4,  7,  6,   // Right face
    10, 9,  8,  8,  11, 10,  // Back face
    14, 13, 12, 12, 15, 14,  // Left face
    18, 17, 16, 16, 19, 18,  // Top face
    22, 21, 20, 20, 23, 22   // Bottom face
}};

// CCW
// constexpr std::array<unsigned int, 36> CUBE_IB{{
//    0,  1,  2,  2,  3,  0,   // Front face
//    4,  5,  6,  6,  7,  4,   // Right face
//    8,  9,  10, 10, 11, 8,   // Back face
//    12, 13, 14, 14, 15, 12,  // Left face
//    16, 17, 18, 18, 19, 16,  // Top face
//    20, 21, 22, 22, 23, 20   // Bottom face
//}};

unsigned int AppendGridData(std::vector<float>& vertex_buffer,
                            std::vector<unsigned int>& index_buffer)
{
    const unsigned int m = 5;
    const unsigned int n = 5;
    const unsigned int face_count = (m - 1) * (n - 1) * 2;

    vertex_buffer.reserve(m * n * 5);
    index_buffer.reserve(face_count * 3);

    const float width = 10.f;
    const float depth = 10.f;

    const float dx = width / (n - 1);
    const float dz = depth / (m - 1);

    const float du = 1.0f / (n - 1);
    const float dv = 1.0f / (m - 1);

    for (unsigned int i = 0; i < m; ++i) {
        float z = (depth / 2) - i * dz;
        for (unsigned int j = 0; j < n; ++j) {
            float x = j * dx - (width / 2);
            vertex_buffer.push_back(x);
            vertex_buffer.push_back(0.0f);
            vertex_buffer.push_back(z);
            vertex_buffer.push_back(j * du);
            vertex_buffer.push_back(i * dv);
        }
    }

    for (unsigned int i = 0; i < m - 1; ++i) {
        for (unsigned int j = 0; j < n - 1; ++j) {
            index_buffer.push_back(i * n + j);
            index_buffer.push_back(i * n + j + 1);
            index_buffer.push_back((i + 1) * n + j);
            index_buffer.push_back((i + 1) * n + j);
            index_buffer.push_back(i * n + j + 1);
            index_buffer.push_back((i + 1) * n + j + 1);
        }
    }
    return face_count * 3;
}

}  // namespace

WindowData GetWindowData()
{
    return WindowData{/*.width =*/800,
                      /*.heigth =*/600,
                      /*.aspect_ratio=*/800.f / 600.f};
}

SceneData GetSceneModel()
{
    SceneData m;
    m.vertex_buffer_data = std::vector<float>(CUBE_VB.begin(), CUBE_VB.end());
    m.index_buffer_data =
        std::vector<unsigned int>(CUBE_IB.begin(), CUBE_IB.end());
    m.submodels.emplace_back(
        SceneData::SubModel{/*.vertex_offset =*/0,
                            /*.index_offset =*/0,
                            /*.index_count =*/CUBE_IB.size()});

    const unsigned int curr_vertex_offset = m.vertex_buffer_data.size() / 5;
    const unsigned int curr_index_offset = m.index_buffer_data.size();

    auto grid_index_count =
        AppendGridData(m.vertex_buffer_data, m.index_buffer_data);
    m.submodels.emplace_back(
        SceneData::SubModel{/*.vertex_offset =*/curr_vertex_offset,
                            /*.index_offset =*/curr_index_offset,
                            /*.index_count =*/grid_index_count});
    return m;
}

}  // namespace GameData
