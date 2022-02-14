-- Copyright (c) 2021 Emmanuel Arias
local ROOT = "../"
include "dependencies.lua"

-- _ACTION is set to nil when premake is run but no generation is needed
-- for example "premake5 -help"
local gen_action = "NULL"
if _ACTION ~= nill then gen_action = _ACTION end
local GEN_FOLDER = ("generated/" .. gen_action .. "/")

-- Global variables
PROJECT_ROOT = "/sources/%{prj.name}/"

local OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.architecture}/"
TARGET_FOLDER = (GEN_FOLDER .. "target/" .. "/%{prj.name}/" .. OUTPUT_DIR)
INTERMEDIATE_FOLDER = (GEN_FOLDER .. "intermediate/" .. "/%{prj.name}/" .. OUTPUT_DIR)

workspace "Tamarindo Engine"
   startproject "game_app"
   filename "tamarindo_engine"
   location (ROOT .. GEN_FOLDER)

   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

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
      include (ROOT .. "sources/engine_lib/external/glad")
      include (ROOT .. "sources/engine_lib/external/glfw")
   group ""

   include (ROOT .. "sources/engine_lib")
   include (ROOT .. "sources/game_app")
