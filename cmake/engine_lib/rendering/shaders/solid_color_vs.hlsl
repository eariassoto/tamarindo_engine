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

struct VertexInput {
    float4 position : POSITION;
};

struct VertexOutput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// VertexOutput main(float4 position : POSITION)
// {
//     VertexOutput output = {position, float4(1.0f, 0.0f, 0.0f, 1.0f)};
//     return output;
// }

VertexOutput main(uint vid
                  : SV_VERTEXID)
{
    VertexOutput output = {float4(vid * 0.5f, vid & 1, 1, 1.5f) - 0.5f,
                           float4(vid == 0, vid == 1, vid == 2, 1)};
    return output;
}

