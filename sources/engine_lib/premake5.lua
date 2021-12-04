-- Copyright (c) 2021 Emmanuel Arias
local ROOT = "../../"

project "engine_lib"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   targetdir (ROOT .. TARGET_FOLDER)
   objdir (ROOT .. INTERMEDIATE_FOLDER)

   files {
       (ROOT .. PROJECT_ROOT .. "engine_lib/**.h" ),
       (ROOT .. PROJECT_ROOT .. "engine_lib/**.cpp" )
    }

    includedirs {
      (ROOT .. PROJECT_ROOT .. "engine_lib")
    }
