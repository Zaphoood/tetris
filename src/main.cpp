#include "SDL.h"
#include "iostream"

#include "active.h"
#include "playfield.h"

int main(int argc, char *argv[]) {
    // Initialize SDL and create window
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow(
        "Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GRID_SIZE_VISIBLE_X * CELL_SIZE, GRID_SIZE_VISIBLE_Y * CELL_SIZE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Playfield playfield = Playfield();
    Active active = Active(0);

    bool isRunning = true;

    while (isRunning) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        playfield.draw(renderer, &active, 0, 0);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
