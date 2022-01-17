# Tamarindo Engine

This project is a work in progress engine for education purposes.

## Getting the code:

To get the code, simply clone the repository:

```
git clone --recurse-submodules git@github.com:eariassoto/tamarindo_engine.git

```

## Building

The respository does not contains any specific configuration file. It uses Premake to define the build configurations and generate the proper solutions. 

### Windows

For Windows, I use Visual Studio 2019. Execute the `scripts/build_project.bat` script. This will create the solution in the `generated/vs2019` folder. After compiling, the libraries and executables will be located in `generated/vs2019/target`.

## Linux

Similar process, on Linux execute the `scripts/build_project.sh` script. This will create the Makefiles inside the `generated/gmake` folder. After that you can compile with `make`. For example:

```
> cd generated/gmake
> make config=debug_x64 -j4 game_app
```

The executable will also be generated in `generated/gmake`:

```
> ./target/game_app/Debug-x86_64/game_app
```
