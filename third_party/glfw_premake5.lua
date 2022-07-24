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

project "glfw"
   kind "StaticLib"
   language "C"
   staticruntime "off"

   targetdir ("../" .. TARGET_FOLDER)
   objdir ("../" .. INTERMEDIATE_FOLDER)

   files {
      "%{prj.name}/include/GLFW/glfw3.h",
      "%{prj.name}/include/GLFW/glfw3native.h",
      "%{prj.name}/src/internal.h",
      "%{prj.name}/src/platform.h",
      "%{prj.name}/src/mappings.h",
      "%{prj.name}/src/context.c",
      "%{prj.name}/src/init.c",
      "%{prj.name}/src/input.c",
      "%{prj.name}/src/monitor.c",
      "%{prj.name}/src/platform.c",
      "%{prj.name}/src/vulkan.c",
      "%{prj.name}/src/window.c",
      "%{prj.name}/src/egl_context.c",
      "%{prj.name}/src/osmesa_context.c",
      "%{prj.name}/src/null_platform.h",
      "%{prj.name}/src/null_joystick.h",
      "%{prj.name}/src/null_init.c",
  
      "%{prj.name}/src/null_monitor.c",
      "%{prj.name}/src/null_window.c",
      "%{prj.name}/src/null_joystick.c"
   }

   filter "system:linux"
      files {
         "%{prj.name}/src/x11_init.c",
         "%{prj.name}/src/x11_monitor.c",
         "%{prj.name}/src/x11_window.c",
         "%{prj.name}/src/xkb_unicode.c",
         "%{prj.name}/src/posix_module.c",
         "%{prj.name}/src/posix_poll.c",
         "%{prj.name}/src/posix_time.c",
         "%{prj.name}/src/posix_thread.c",
         "%{prj.name}/src/glx_context.c",
         "%{prj.name}/src/egl_context.c",
         "%{prj.name}/src/osmesa_context.c",
         "%{prj.name}/src/linux_joystick.c"
      }

      defines {
         "_GLFW_X11"
      }

   filter "system:windows"
      files {
         "%{prj.name}/src/win32_init.c",
         "%{prj.name}/src/win32_module.c",
         "%{prj.name}/src/win32_joystick.c",
         "%{prj.name}/src/win32_monitor.c",
         "%{prj.name}/src/win32_time.h",
         "%{prj.name}/src/win32_time.c",
         "%{prj.name}/src/win32_thread.h",
         "%{prj.name}/src/win32_thread.c",
         "%{prj.name}/src/win32_window.c",
         "%{prj.name}/src/wgl_context.c",
         "%{prj.name}/src/egl_context.c",
         "%{prj.name}/src/osmesa_context.c"
      }

      defines { 
         "_GLFW_WIN32",
         "_CRT_SECURE_NO_WARNINGS"
      }

      links {
         "Dwmapi.lib"
      }
