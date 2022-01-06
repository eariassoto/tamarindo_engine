// Copyright(c) 2022 Emmanuel Arias
#pragma once

#include "rendering/mesh.h"

#include <memory>

namespace tamarindo
{
class Mesh;

namespace GeometryGenerator
{
	std::unique_ptr<Mesh> createSquare();

}  // namespace GeometryGenerator

}  // namespace tamarindo
