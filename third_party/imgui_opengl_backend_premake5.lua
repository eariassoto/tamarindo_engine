
--[[
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
]]
project "imgui_opengl_backend"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir ("../" .. TARGET_FOLDER)
   objdir ("../" .. INTERMEDIATE_FOLDER)

   files {
      "imgui/backends/imgui_impl_glfw.cpp",
      "imgui/backends/imgui_impl_glfw.h",
      "imgui/backends/imgui_impl_opengl3.cpp",
      "imgui/backends/imgui_impl_opengl3.h",
   }

   includedirs {
      "imgui",
      "imgui_backends",
      "%{ThirdPartyIncludeDir.GLFW}",
   }

   links {
      "imgui",
   }
