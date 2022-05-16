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

include "dependencies.lua"

-- _ACTION is set to nil when premake is run but no generation is needed
-- for example "premake5 -help"
local gen_action = "NULL"
if _ACTION ~= nill then gen_action = _ACTION end
local GEN_FOLDER = ("generated/" .. gen_action .. "/")

-- Global variables
PROJECT_ROOT = "../../sources/%{prj.name}/"

local OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.architecture}/"
TARGET_FOLDER = ("../../" .. GEN_FOLDER .. "output/" .. "/%{prj.name}/" .. OUTPUT_DIR)
INTERMEDIATE_FOLDER = ("../../" .. GEN_FOLDER .. "intermediate/" .. "/%{prj.name}/" .. OUTPUT_DIR)

workspace "Tamarindo Engine"
   startproject "tamarindo_editor"
   filename "tamarindo_engine"
   location (GEN_FOLDER)

   configurations { "Debug", "Release" }
   platforms { "x64", "x32" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      runtime "Debug"
      symbols  "On"
   
   filter "configurations:Release"
      defines { "NDEBUG" }
      runtime "Release"
      optimize "On"

   filter "platforms:x64"
      architecture "x86_64"

   filter "platforms:x32"
      architecture "x86"
   
   filter "system:windows"
      defines { "TM_PLATFORM_WINDOWS" }
   
   filter "system:linux"
      defines { "TM_PLATFORM_LINUX" }

   group "Dependencies"
      include ("third_party/glad_premake5.lua")
      include ("third_party/glfw_premake5.lua")
      --include ("third_party/googletest_premake5.lua")
      include ("third_party/imgui_premake5.lua")
      include ("third_party/imgui_opengl_backend_premake5.lua")
   group ""

   include ("sources/engine_lib")
   --include ("sources/engine_lib/tests_premake5.lua")
   include ("sources/tamarindo_editor")
