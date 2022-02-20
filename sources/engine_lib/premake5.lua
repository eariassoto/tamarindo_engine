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

local ROOT = "../../"

project "engine_lib"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir (ROOT .. TARGET_FOLDER)
   objdir (ROOT .. INTERMEDIATE_FOLDER)

   files {
      (ROOT .. PROJECT_ROOT .. "engine_lib/**.h"),
      (ROOT .. PROJECT_ROOT .. "engine_lib/**.cpp"),
      (ROOT .. PROJECT_ROOT .. "external/glm/glm/**.hpp"),
      (ROOT .. PROJECT_ROOT .. "external/glm/glm/**.inl")
   }

   defines {
      "GLFW_INCLUDE_NONE",
      "GLM_FORCE_LEFT_HANDED"
   }

   includedirs {
      (ROOT .. PROJECT_ROOT .. "engine_lib"),
      (ROOT .. "%{IncludeDir.spdlog}"),
      (ROOT .. "%{IncludeDir.glad}"),
      (ROOT .. "%{IncludeDir.GLFW}"),
      (ROOT .. "%{IncludeDir.GLM}")
   }

   flags {
      "FatalWarnings",
      "MultiProcessorCompile"
   }

   filter "system:linux"
      pic "On"

      systemversion "latest"

      defines { "_X11" }
