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

local EXT_GEN_FOLDER = ("../generated/" .. gen_action .. "/")

EXT_TARGET_FOLDER = (EXT_GEN_FOLDER .. "third_party_output/%{cfg.buildcfg}-%{cfg.architecture}")
EXT_INTERMEDIATE_FOLDER = (EXT_GEN_FOLDER .. "third_party_intermediate/%{prj.name}/%{cfg.buildcfg}-%{cfg.architecture}")

workspace "Tamarindo Engine Dependencies"
   filename "tamarindo_engine_third_party"
   location (EXT_GEN_FOLDER)

   configurations { "Debug", "Release" }
   platforms { "x64"}

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

   include ("glad_premake5.lua")
   include ("glfw_premake5.lua")
   include ("imgui_premake5.lua")
   include ("imgui_opengl_backend_premake5.lua")
