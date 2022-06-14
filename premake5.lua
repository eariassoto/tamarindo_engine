--[[
Copyright 2021-2022 Emmanuel Arias Soto

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
]]

IncludeDir = {}
IncludeDir["spdlog"]         = "../../third_party/spdlog/include"
IncludeDir["glad"]           = "../../third_party/glad/include"
IncludeDir["GLFW"]           = "../../third_party/glfw/include"
IncludeDir["GLM"]            = "../../third_party/glm"
IncludeDir["googletest"]     = "../../third_party/googletest/googletest/include"
IncludeDir["imgui"]          = "../../third_party/imgui"
IncludeDir["imgui_backends"] = "../../third_party/imgui/backends"
IncludeDir["tinygltf"]       = "../../third_party/tinygltf"

ThirdPartyIncludeDir = {}
ThirdPartyIncludeDir["GLFW"] = "../third_party/glfw/include"

-- _ACTION is set to nil when premake is run but no generation is needed
-- for example "premake5 -help"
gen_action = "NULL"
if _ACTION ~= nill then gen_action = _ACTION end

include "third_party"
include "sources"
