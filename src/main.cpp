#include "SDL.h"
#include "chrono"
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
    uint8_t type = 0;
    Active active = Active(type);

    bool isRunning = true;

    while (isRunning) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                case SDLK_RIGHT:
                    active.moveRight(&playfield);
                    break;
                case SDLK_LEFT:
                    active.moveLeft(&playfield);
                    break;
                case SDLK_DOWN:
                    active.stepDown(&playfield);
                    break;
                case SDLK_UP:
                    active.m_y--;
                    break;
                case SDLK_SPACE:
                    type = (type + 1) % 7;
                    active.setAndRespawn(type, &playfield);
                    break;
                }
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        playfield.draw(renderer, 0, 0);
        active.draw(renderer, &playfield);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
