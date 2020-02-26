# Procedural Terrain - 3D Voxel Engine

## This is my final year project that I have decided to do, creating a Procedural Generation of Voxel Terrain from scratch using OpenGL.

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Visual Studio 2017/19
Included with Project:
glfw3.3 library

## Development Documentation
Project Management - https://tasks.toodledo.com/main/-9

Weekly Timetable Plan - https://liveteesac-my.sharepoint.com/:x:/g/personal/s6099298_live_tees_ac_uk/ETa8nrw_5RRCouLaw_GlA_cBaqXsIdxXXlt-Yb8eyrPjDw?e=0Xmgor

OpenGL Documentation - http://docs.gl/

GLM API Documentation - https://glm.g-truc.net/0.9.9/api/modules.html

Rendering Voxels Article - https://dev.to/superfola/lets-render-chunks-of-voxels-with-opengl--47g9

## DevLog

* [29/10/2019] - Setup IDE, linked glfw 3.3 successfully with Visual Studio 2017(IDE), 
using the static library version of glfw(can be changed to Dynamic Link Library if needed in the future) and used legacy glfw 1.1 to draw a simple triangle to screen.
* [30/10/2019] - I have successfully linked glew 2.1.0 which links all the declariations and points to the functions in the GPU Driver, which is where the latest version of OpenGL is stored, since we use the GPU, the manufacturer
has is the one that has modern OpenGl 4.6(My Version Nvidia) and GLEW is a library that links all those toghether to be able to use modern OpenGl, since the lastest version on windows is OpenGl 1.1(Legacy).
*[31/10/2019] - Linked support of OpenGL with both Win32(32bit) and x64(64bit) and added new project(Genesis Engine).
*[06/02/2020] - Started implementing basic 2d rendering with modern OpenGL e.g. triangles, squares etc.
### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Visual Studio](https://visualstudio.microsoft.com/vs/) - The Intergrated Developer Enviroment(IDE) Used for this Project
* [glfw 3.3](https://www.glfw.org/) - The Library used for this Project
* [glm 0.9.9](https://glm.g-truc.net/0.9.9/index.html) - The maths library that is based on OpenGL
* [ImGui](https://github.com/ocornut/imgui) - Graphics interface for OpenGL

## Author

* **Michael Thomas** - 

