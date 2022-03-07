:: Copyright (c) 2021 Emmanuel Arias
@echo off

del /f /s /q generated
call third_party\premake\bin\win64\premake5.exe --file=premake5.lua vs2019
