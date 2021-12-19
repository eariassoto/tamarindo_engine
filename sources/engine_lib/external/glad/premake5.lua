local ROOT = "../../../../"

project "glad"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir (ROOT .. TARGET_FOLDER)
   objdir (ROOT .. INTERMEDIATE_FOLDER)

   files {
      "include/glad/glad.h",
      "include/KHR/khrplatform.h",
      "src/glad.c"
   }

   includedirs {
      "include"
   }

   filter "system:windows"
      systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
