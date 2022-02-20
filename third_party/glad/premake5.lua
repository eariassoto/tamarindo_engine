
project "glad"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir (TARGET_FOLDER)
   objdir (INTERMEDIATE_FOLDER)

   files {
      "include/glad/glad.h",
      "include/KHR/khrplatform.h",
      "src/glad.c"
   }

   includedirs {
      "include"
   }

   filter "system:linux"
		pic "On"

		systemversion "latest"

		defines {
			"_GLAD_X11"
		}

   filter "system:windows"
		systemversion "latest"

		defines { 
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
