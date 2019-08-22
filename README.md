# Engine1

## About
Engine1 is [a 2D/3D graphics engine project](https://github.com/vovojta/Engine1/). Engine1's goal is to be a library that structures and manages the inner workings of a game, with abstractions to different APIs and platforms.

## 1. Engine1 support

Engine1 is in active development. Here is a short list of what is supported and what isn't. This can change at any time.

### 1.1 Supported platforms
Currently Engine1 supports:

- Computer OS:
  - ![Windows supported](https://img.shields.io/badge/Windows-win--64-green.svg)
  - ![Linux not supported](https://img.shields.io/badge/Linux-Not%20Supported-red)
  - ![MacOS not supported](https://img.shields.io/badge/MacOS-Not%20Supported-red.svg)
- Mobile OS:
  - ![Android not supported](https://img.shields.io/badge/Android-Not%20Supported-red.svg)
  - ![IOS not supported](https://img.shields.io/badge/IOS-Not%20Supported-red.svg)


### 1.2 Hardware requirements
As of now, Engine1 only supports OpenGL Rendering and requires a minimum version of 4.5.

## 2 Installing and setup

Start by cloning the repository with `git clone --recursive <URL>`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.
Engine1 uses _Premake 5_ as a build generation tool. Visit the [Premake website](https://premake.github.io/download.html) to download and install it.

Next: Follow the steps relevant to your operating system.

### 2.1 Windows

Premake 5.0.0-alpha14 is provided as [premake5.exe](https://github.com/vovojta/Engine1/blob/master/vendor/bin/premake/premake5.exe) in the repository. Execute and follow the install instructions.

Premake generates project files for Visual Studio. To generate the `.sln` and `.vcxproj` files for Visual Studio 2019, run `premake vs2019` at the command line. Or you may run [GenerateProjects.bat](https://github.com/vovojta/Engine1/blob/master/GenerateProjects.bat) as a convenience batch file for this task.
