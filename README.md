# Procedural Terrain - 3D Voxel Engine

One Paragraph of project description goes here

## This is my final year project that I have decided to do, creating a 3D Graphics Renderer from scratch using OpenGL.

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Visual Studio 2017/19
Included with Project:
glfw3.3 library

```
Give examples
```
## Development Documentation
Project Management - https://procedural-generation-of-voxel-terrain.monday.com/boards/438700509 

Weekly Timetable Plan - https://liveteesac-my.sharepoint.com/:x:/g/personal/s6099298_live_tees_ac_uk/ETa8nrw_5RRCouLaw_GlA_cBaqXsIdxXXlt-Yb8eyrPjDw?e=0Xmgor

## DevLog

* [29/10/2019] - Setup IDE, linked glfw 3.3 successfully with Visual Studio 2017(IDE), 
using the static library version of glfw(can be changed to Dynamic Link Library if needed in the future) and used legacy glfw 1.1 to draw a simple triangle to screen.
* [30/10/2019] - I have successfully linked glew 2.1.0 which links all the declariations and points to the functions in the GPU Driver, which is where the latest version of OpenGL is stored, since we use the GPU, the manufacturer
has is the one that has modern OpenGl 4.6(My Version Nvidia) and GLEW is a library that links all those toghether to be able to use modern OpenGl, since the lastest version on windows is OpenGl 1.1(Legacy).
*[31/10/2019] - Linked support of OpenGL with both Win32(32bit) and x64(64bit) and added new project(Genesis Engine).
*[Next] -  Start rendering 2d shapes to the screen from scratch.
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


## Author

* **Michael Thomas** - 

