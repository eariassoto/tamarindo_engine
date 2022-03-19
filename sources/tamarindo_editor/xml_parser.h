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
#pragma once

#include "engine_lib/rendering/rendering_asset.h"
#include "engine_lib/world/scene.h"
#include "project.h"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

using namespace tamarindo;
namespace fs = std::filesystem;

namespace xmlparser
{
std::unique_ptr<Project> parseProjectFolder(const fs::path& folder_path,
                                            const std::string& file_name);

std::unique_ptr<Scene> parseSceneFromFile(const Project& project);

[[nodiscard]] std::vector<RenderingAsset*> parseDataFromFile(
    const Project& project);

}  // namespace xmlparser
