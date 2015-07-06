# Memrex

MemRex is a Playstation Memory Card viewer and editor, primarily desiged for devices such as the Raspberry Pi.
This is a work in progress.

Supported Image Formats
=======================

**Raw Images**
*  SmartLink (.PSM), WinPSM (.PS), DataDeck (.DDF), FPSX (.MCR), ePSXe (.MCD)

**Headered Images**
*  VGS (.MEM)
*  DexDrive (.GME)

**Single-slot saves**
Single-slot saves may be eventually supported.

Building
========

You will need the following tools/libraries to compile MemRex:

* CMake 
* SDL2 
* SDL_image 2.0

**Visual Studio 201x:**

Download CMake (http://www.cmake.org/download/) and extract to a folder and set your system evironment PATH to that folder. If you already have an existing CYGWIN or MinGW installation it might make sence to drop it into the bin folder of that environment.

Download SDL2 Runtime Binaries and VC Development Libraries (https://www.libsdl.org/download-2.0.php)

Download SDL_image 2.0 Runtime Binaries and VC Development Libraries (https://www.libsdl.org/projects/SDL_image/)

Create a folder and extract the contents of the SDL2 and SDL2_image VC Development Libraries into this folder. This folder will be your *path to SDL2*

Create a folder named *bin* in the SDL2 folder and extract the contents of the SDL2 and SDL2_image Runtime Binaries into the bin folder. The SDL2 folder should now contain the folloing folders:

```
bin
include
lib
```

You should now be ready to generate the Memrex solution. Navigate to the Memrex folder and drop into a command prompt.

*NOTE:* It is recommended to use an out-of-source build to keep your source repository clean.

```cmd
md build
cd build
SET SDL_WIN32_PATH=<path to SDL2>
cmake ..
```

This should generate the necessary Visual Studio projects and solution required to build Memrex. A post-build step is included that will copy the necessary DLLs from your SDL2 path to your output folder (e.g. Debug).

**Debian/Ubuntu:**

```bash
sudo apt-get install libsdl2-dev libgl1-mesa-dev build-essential cmake
```

Generate and build the Makefile with CMake:

```bash
cmake .
make
```

Usage
=====

Although a folder navigation GUI is planned, Memrex currently loads the memory card image from the first argument passed.

```cmd
memrex <path_to_memorycard_image>
```
