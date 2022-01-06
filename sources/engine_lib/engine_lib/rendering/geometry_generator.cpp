// Copyright(c) 2022 Emmanuel Arias
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
