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
<<<<<<<< HEAD:sources/tamarindo_editor/xml_scene_parser.h
#pragma once

#include "engine_lib/world/scene.h"

#include <memory>
========

#pragma once

>>>>>>>> 2f0691d (wip):sources/engine_lib/engine_lib/rendering/rendering_asset.h
#include <string>

using namespace tamarindo;

namespace xmlparser
{
<<<<<<<< HEAD:sources/tamarindo_editor/xml_scene_parser.h
std::unique_ptr<Scene> parseSceneFromStrBuffer(const std::string& xml_doc);
std::unique_ptr<Scene> parseSceneFromFile(const std::string& file_path);
}
========

class RenderingAsset
{
   public:
    RenderingAsset() = delete;

    RenderingAsset(const std::string& name): m_Name(name) {}

    virtual ~RenderingAsset() = default;

   private:
    std::string m_Name;
};

}  // namespace tamarindo
>>>>>>>> 2f0691d (wip):sources/engine_lib/engine_lib/rendering/rendering_asset.h
