
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
project "glad"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir ("../" .. TARGET_FOLDER)
   objdir ("../" .. INTERMEDIATE_FOLDER)

   files {
      "%{prj.name}/include/glad/glad.h",
      "%{prj.name}/include/KHR/khrplatform.h",
      "%{prj.name}/src/glad.c"
   }

   includedirs {
      "%{prj.name}/include"
   }

   filter "system:linux"
		defines {
			"_GLAD_X11"
		}

   filter "system:windows"
		defines { 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
