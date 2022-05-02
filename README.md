# Tetris
A simple Tetris clone written in C++ using the SDL2 library.

## Dependencies

You'll need gcc and make for compiling and the [SDL2](https://github.com/libsdl-org/SDL) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) libraries.

SDL2 and SDL2_ttf can be installed using apt:
```
sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```
Or you can just install them from source.

## Compiling
In the root folder of the project, run
```
mkdir build && cd build
cmake ..
make -j$(nproc)
```
