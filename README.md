# Tetris

A simple Tetris clone written in C++ using the SDL2 library.

## Dependencies

You'll need `gcc` and `make` for compiling, as well as the [SDL2](https://github.com/libsdl-org/SDL) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) libraries.

On Ubuntu, SDL2 and SDL2_ttf can be installed with `apt`:

```sh
sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```

Alternatively, you can just install from source (see the links above for details).

## Compiling

In the root directory of the repository, run

```sh
cmake -B./build && cd build
make -j$(nproc)
```

To install `tetris` to `/usr/local/bin`, run `sudo make install` from the `build` directory.
