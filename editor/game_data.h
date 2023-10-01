/*
 Copyright 1023 Emmanuel Arias Soto

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

#ifndef TAMARINDO_EDITOR_GAME_DATA_H_
#define TAMARINDO_EDITOR_GAME_DATA_H_

#include <vector>

constexpr float BACKGROUND_COLOR[4] = {0.678f, 0.749f, 0.796f, 1.0f};

constexpr const char SHADER_CODE[] = R"(
cbuffer ViewProjectionBuffer
{
    matrix viewProjection;
};

struct VertexInput
{
    float3 position : POSITION;
    float2 tex : TEX;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEX;
};

PixelInput vs(VertexInput input)
{
    PixelInput output;

    output.position = mul(float4(input.position, 1.0f), viewProjection);
    output.tex = input.tex;

    return output;
}

float4 ps(PixelInput input) : SV_TARGET
{
    return float4(input.tex, 0.0f, 1.0f);
}
)";

namespace GameData
{
struct WindowData {
    unsigned int width;
    unsigned int height;
    float aspect_ratio;
};

struct SceneData {
    struct SubModel {
        unsigned int vertex_offset;
        unsigned int index_offset;
        unsigned int index_count;
    };

    std::vector<float> vertex_buffer_data;

    std::vector<unsigned int> index_buffer_data;

    std::vector<SubModel> submodels;
};

WindowData GetWindowData();

SceneData GetSceneModel();

}  // namespace GameData

#endif  // TAMARINDO_EDITOR_GAME_DATA_H_
