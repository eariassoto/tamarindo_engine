-- Copyright (c) 2021 Emmanuel Arias
local ROOT = "../../"

project "game_app"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir (ROOT .. TARGET_FOLDER)
   objdir (ROOT .. INTERMEDIATE_FOLDER)

   files {
      (ROOT .. PROJECT_ROOT .. "**.h" ),
      (ROOT .. PROJECT_ROOT .. "**.cpp" )
   }

   includedirs {
      "../engine_lib/",
      "../engine_lib/engine_lib",
      (ROOT .. "%{IncludeDir.spdlog}"),
      (ROOT .. "%{IncludeDir.glad}"),
      (ROOT .. "%{IncludeDir.GLFW}"),
      (ROOT .. "%{IncludeDir.GLM}")
   }

   links {
      "engine_lib",
      "GLFW",
      "glad"
   }

   filter "system:linux"
      links {
         "GL",
         "dl",
         "pthread"
      }
