:: Copyright (c) 2022 Emmanuel Arias
@echo off

del /f /s /q generated
call third_party/premake\bin\win64\premake5.exe --file=premake5.lua vs2022
