
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
project "googletest"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir ("../" .. TARGET_FOLDER)
   objdir ("../" .. INTERMEDIATE_FOLDER)

   files {
      "%{prj.name}/googletest/src/gtest_main.cc",
      "%{prj.name}/googletest/src/gtest-assertion-result.cc",
      "%{prj.name}/googletest/src/gtest-death-test.cc",
      "%{prj.name}/googletest/src/gtest-filepath.cc",
      "%{prj.name}/googletest/src/gtest-matchers.cc",
      "%{prj.name}/googletest/src/gtest-internal-inl.h",
      "%{prj.name}/googletest/src/gtest-port.cc",
      "%{prj.name}/googletest/src/gtest-printers.cc",
      "%{prj.name}/googletest/src/gtest-test-part.cc",
      "%{prj.name}/googletest/src/gtest-typed-test.cc",
      "%{prj.name}/googletest/src/gtest.cc",
      "%{prj.name}/googletest/include/**.h"
   }

   includedirs {
      "%{prj.name}/googletest/",
      "%{prj.name}/googletest/include"
   }
