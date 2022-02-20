/*
 Copyright 2022 Emmanuel Arias Soto

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

#include "geometry_generator.h"

namespace tamarindo
{
std::unique_ptr<Mesh> GeometryGenerator::createSquare()
{
    std::unique_ptr<Mesh> square = std::make_unique<Mesh>(4, 6);

    square->addVertex(+0.5f, +0.5f, 0.0f, 1.0f, 0.0f);  // top right
    square->addVertex(+0.5f, -0.5f, 0.0f, 1.0f, 1.0f);  // bottom right
    square->addVertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f);  // bottom left
    square->addVertex(-0.5f, +0.5f, 0.0f, 0.0f, 0.0f);  // top left
    square->addIndex(0);                                // first triangle
    square->addIndex(1);
    square->addIndex(3);
    square->addIndex(1);  // second triangle
    square->addIndex(2);
    square->addIndex(3);

    return std::move(square);
}

}  // namespace tamarindo
