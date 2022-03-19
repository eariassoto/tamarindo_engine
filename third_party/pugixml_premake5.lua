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
project "pugixml"
   kind "StaticLib"
   language "C++"
   staticruntime "off"

   targetdir (TARGET_FOLDER)
   objdir (INTERMEDIATE_FOLDER)

   files {
      "%{prj.name}/src/pugiconfig.hpp",
      "%{prj.name}/src/pugixml.cpp",
      "%{prj.name}/src/pugixml.hpp"
   }

   includedirs {
       "%{prj.name}/src"
   }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
