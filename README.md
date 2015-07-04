# Memrex

MemRex is a Playstation Memory Card viewer and editor, primarily desiged for devices such as the Raspberry Pi.
This is a work in progress.

Building
========

You will need the following tools/libraries to compile MemRex:

* CMake 
* SDL2

**Debian/Ubuntu:**

```bash
sudo apt-get install libsdl2-dev libgl1-mesa-dev build-essential cmake
```

Generate and build the Makefile with CMake:
```bash
cmake .
make
```
