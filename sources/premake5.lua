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

GEN_FOLDER = ("../generated/" .. gen_action .. "/")

-- Global variables
PROJECT_ROOT = "../../sources/%{prj.name}/"

TARGET_FOLDER = ("../" .. GEN_FOLDER .. "output/%{cfg.buildcfg}-%{cfg.architecture}")
INTERMEDIATE_FOLDER = ("../../" .. GEN_FOLDER .. "intermediate//%{prj.name}/%{cfg.buildcfg}-%{cfg.architecture}")

workspace "Tamarindo Engine"
   startproject "tamarindo_editor"
   filename "tamarindo_engine"
   location (GEN_FOLDER)

   configurations { "Debug", "Release" }
   platforms { "x64" }

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
   
   filter "system:windows"
      defines { "TM_PLATFORM_WINDOWS" }
   
   filter "system:linux"
      defines { "TM_PLATFORM_LINUX" }

   include ("engine_lib")
   --include ("engine_lib/tests_premake5.lua")
   include ("tamarindo_editor")
