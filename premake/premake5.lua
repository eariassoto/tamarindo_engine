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
      symbols  "On"
   
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter "platforms:x64"
      architecture "x86_64"

   filter "platforms:x32"
      architecture "x86"

   flags {
      "FatalWarnings",
      "MultiProcessorCompile"
   }

   include (ROOT .. "sources/engine_lib")
   include (ROOT .. "sources/game_app")
