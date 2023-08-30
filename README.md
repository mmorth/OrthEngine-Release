# OrthEngine-Release
OrthEngine is a Graphics Engine written in C++ and OpenGL that currently provides the following basic functionality:
- Basic object rendering
- Basic lighting (shaders, textures, etc.)
- Player Movement
- Skybox and transparent object rendering
- Unit and integration testing coverage
- g3log logging
- Physics collision support

The following future features are planned:
- Terrain generation
- Engine user GUI
- Advanced lighting (PBR, Ray Tracing)
- Logging, CI, Docker, Linter Support
- 2D graphics support
- Networking

## Installation and Running
There is a zip file called "Externals" in the Resources folder that contains all the required depdendencies (except for the vcpkg-installed ones below). By default, CMake looks for these in C:\Externals and vcpkg dependencies in C:\workspace\Externals\vcpkg, but this can be changed if needed in the root-level CMakeLists.txt.

If you wish to compile the dependencies yourself, the following are required:
- Assimp
- g3log
- eigen
- freetype
- glad
- GLFW
- glm
- json
- KHR
- Bullet (use vcpkg to install)

Note that a Docker container will eventually be created for OrthEngine to better manage these dependencies!

## Resources
Below is the sample scene that is rendering when first running OrthEngine:
![OrthEngine](https://github.com/mmorth/OrthEngine-Release/blob/master/Resources/OrthEngine.png)

## Contributors
- Matthew Orth
