
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
project "imgui"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir (EXT_TARGET_FOLDER)
   objdir (EXT_INTERMEDIATE_FOLDER)

   files {
      "%{prj.name}/imconfig.h",
      "%{prj.name}/imgui.cpp",
      "%{prj.name}/imgui.h",
      "%{prj.name}/imgui_demo.cpp",
      "%{prj.name}/imgui_draw.cpp",
      "%{prj.name}/imgui_internal.h",
      "%{prj.name}/imgui_tables.cpp",
      "%{prj.name}/imgui_widgets.cpp",
      "%{prj.name}/imstb_rectpack.h",
      "%{prj.name}/imstb_textedit.h",
      "%{prj.name}/imstb_truetype.h",
   }

   includedirs {
      "%{prj.name}"
   }

   filter "system:linux"
		pic "On"
		systemversion "latest"

   filter "system:windows"
		systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
