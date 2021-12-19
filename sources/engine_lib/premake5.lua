-- Copyright (c) 2021 Emmanuel Arias
local ROOT = "../../"

project "engine_lib"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir (ROOT .. TARGET_FOLDER)
   objdir (ROOT .. INTERMEDIATE_FOLDER)

   files {
      (ROOT .. PROJECT_ROOT .. "engine_lib/**.h" ),
      (ROOT .. PROJECT_ROOT .. "engine_lib/**.cpp" )
   }

   defines {
      "GLFW_INCLUDE_NONE"
   }

   includedirs {
      (ROOT .. PROJECT_ROOT .. "engine_lib"),
      (ROOT .. "%{IncludeDir.spdlog}"),
      (ROOT .. "%{IncludeDir.glad}"),
      (ROOT .. "%{IncludeDir.GLFW}")
   }

   links {
      "glad",
      "GLFW",
      "opengl32.lib"
   }
