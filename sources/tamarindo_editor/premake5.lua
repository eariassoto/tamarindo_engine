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

project "tamarindo_editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir (TARGET_FOLDER)
   objdir (INTERMEDIATE_FOLDER)

   files {
      (PROJECT_ROOT .. "**.h" ),
      (PROJECT_ROOT .. "**.cc")
   }

   includedirs {
      "../engine_lib/",
      "../engine_lib/engine_lib",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.glad}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.imgui}",
      "%{IncludeDir.imgui_backends}",
      "%{IncludeDir.tinygltf}"
   }

   links {
      "engine_lib",
      "GLFW",
      "glad",
      "imgui",
      "imgui_opengl_backend"
   }

   filter "system:windows"
      defines { "_CRT_SECURE_NO_WARNINGS" }

   flags {
      "FatalWarnings",
      "MultiProcessorCompile"
   }

   filter "system:linux"
      links {
         "GL",
         "dl",
         "pthread"
      }
