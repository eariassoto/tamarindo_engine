# Tamarindo Engine

This project is a work in progress engine for education purposes.

## Getting the code:

To get the code, simply clone the repository:

```
git clone --recurse-submodules git@github.com:eariassoto/tamarindo_engine.git

```

## Building

To create the solution files in Windows use the `scripts/build_project.bat` script. This will create a VS2019 solution in the `generated/` folder. You can also call premake directly to generate other types of build files. For example:

```
premake\bin\win64\premake5.exe --file=premake/premake5.lua gmake
```
