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

#ifndef TAMARINDO_EDITOR_RENDERING_DATA_H_
#define TAMARINDO_EDITOR_RENDERING_DATA_H_

namespace tamarindo
{

extern constexpr const char SHADER_CODE[] = R"(
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

extern constexpr unsigned int MODEL_STRIDE = sizeof(float) * 5;

extern constexpr float TRIANGLE_VB[] = {
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
};

extern constexpr unsigned int TRIANGLE_VB_SIZE = sizeof(TRIANGLE_VB);

extern constexpr unsigned int TRIANGLE_IB[] = {
    0,  1,  2,  2,  3,  0,   // Front face
    4,  5,  6,  6,  7,  4,   // Right face
    8,  9,  10, 10, 11, 8,   // Back face
    12, 13, 14, 14, 15, 12,  // Left face
    16, 17, 18, 18, 19, 16,  // Top face
    20, 21, 22, 22, 23, 20   // Bottom face
};

extern constexpr unsigned int TRIANGLE_IB_SIZE = sizeof(TRIANGLE_IB);

extern constexpr unsigned int TRIANGLE_IB_COUNT =
    TRIANGLE_IB_SIZE / sizeof(unsigned int);

}  // namespace tamarindo

#endif  // TAMARINDO_EDITOR_RENDERING_DATA_H_
