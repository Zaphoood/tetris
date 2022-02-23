#include "iostream"
#include "SDL.h"

#include "playfield.h"

int main(int argc, char *argv[])
{
    // Initialize SDL and create window
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("Tetris",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          600, 400, 0);

    Playfield playfield = Playfield();

    bool isRunning = true;

    while (isRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
