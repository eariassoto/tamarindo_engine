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

project "engine_lib"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir (TARGET_FOLDER)
   objdir (INTERMEDIATE_FOLDER)

   files {
      (PROJECT_ROOT .. "engine_lib/**.h"),
      (PROJECT_ROOT .. "engine_lib/**.cc"),
      (PROJECT_ROOT .. "../../third_party/glm/glm/**.hpp"),
      (PROJECT_ROOT .. "../../third_party/glm/glm/**.inl")
   }

   removefiles {
      (PROJECT_ROOT .. "engine_lib/**_test.h"),
      (PROJECT_ROOT .. "engine_lib/**_test.cc")
   }

   defines {
      "GLFW_INCLUDE_NONE",
      "GLM_FORCE_LEFT_HANDED"
   }

   includedirs {
      (PROJECT_ROOT .. "engine_lib"),
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.glad}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.GLM}",
      "%{IncludeDir.imgui}",
      "%{IncludeDir.imgui_backends}",
      "%{IncludeDir.tinygltf}"
   }

   flags {
      "FatalWarnings",
      "MultiProcessorCompile"
   }

   filter "system:linux"
      pic "On"

      systemversion "latest"

      defines { "_X11" }
   
   filter "system:windows"
      defines { "_CRT_SECURE_NO_WARNINGS" }
      disablewarnings { "4996" }

-- project "engine_lib_tests"
--    kind "ConsoleApp"
--    language "C++"
--    cppdialect "C++17"
--    staticruntime "off"

--    targetdir (TARGET_FOLDER)
--    objdir (INTERMEDIATE_FOLDER)

--    files {
--       "../../sources/engine_lib/engine_lib/**_test.cc"
--    }

--    includedirs {
--       "%{IncludeDir.GLM}",
--       "%{IncludeDir.googletest}",
--       "%{IncludeDir.tinygltf}",
--       "%{IncludeDir.spdlog}",
--       "../../sources/engine_lib",
--       "../../sources/engine_lib/engine_lib"
--    }

--    links {
--       "engine_lib",
--       "googletest"
--    }

--    flags {
--       "FatalWarnings",
--       "MultiProcessorCompile"
--    }

--    filter "system:linux"
--       pic "On"

--       systemversion "latest"

--    filter "system:windows"
--       defines { "_CRT_SECURE_NO_WARNINGS" }
--       disablewarnings { "4996" }
