# OrthEngine-Release
OrthEngine is a Graphics Engine written in C++ and OpenGL that currently provides the following basic functionality:
- Basic object rendering
- Basic lighting (shaders, textures, etc.)
- Player Movement
- Skybox and transparent object rendering
- Unit testing coverage

The following future features are planned:
- Physics support (collisions, gravity, etc.)
- Terrain generation
- Engine user GUI
- Advanced lighting (PBR, Ray Tracing)
- Logging, CI, Docker, Linter Support
- 2D graphics support
- Networking

## Installation and Running
There is a zip file called "Externals" in the Resources folder that contains all the required depdendencies. By default, CMake looks for these in C:\Externals, but this can be changed if needed.

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

Note that a Docker container will eventually be created for OrthEngine to better manage these dependencies!

## Resources
Below is the sample scene that is rendering when first running OrthEngine:
![OrthEngine](https://github.com/mmorth/OrthEngine-Release/blob/master/Resouorces/OrthEngine.png)

## Contributors
- Matthew Orth
