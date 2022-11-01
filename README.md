# Tamarindo Engine

This project is a work in progress toy rendering engine written in modern C++ and OpenGL as rendering framework.

The main goal of this project is to learn about rendering techniques, graphic APIs, resources management, and overall system design withing graphics programming.

## Current status

The engine static library can be used to create a graphical applications and render meshes. Currently, the application creates a Scene with a single root game object, and a camera. The game object can load a Model from a [glTF](https://www.khronos.org/gltf/) file.

The API is a work-in-progress. The engine library is not meant to be yet usable by other applications.

| ![Demo](./demo.png)                                              |
|:----------------------------------------------------------------:|
| 3D asset from: [kenney.nl](https://www.kenney.nl/assets/car-kit) |

This project requires:

- CMake 3.12 or better; 3.14+ highly recommended.
- A C++17 compatible compiler.
- Git.

## Building the project

To configure the project:

```
cmake -S . -B build
```

Add `-GNinja` if you have Ninja.

To build:

```
cmake --build build
```

Add `-j <N>` to build with N paraller jobs.

To use an IDE, such as Visual Studio:

```
cmake -S . -B vs22 -G"Visual Studio 17 2022"
cmake --open vs22
```

# Next improvements

* Better API for application code
* Scene-based projects
* Resource management
* ImGui based UI
* Support different frameworks (DirectX, Vulkan)
* Advanced shading and lighting techniques
