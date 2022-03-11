#include "chrono"
#include "iostream"

#include "SDL.h"

#include "game.h"

int main(int argc, char *argv[]) {
    // Initialize SDL and create window
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window =
        SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, WINDOW_X, WINDOW_Y, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Game game;

    bool isRunning = true;
    game.init();
    while (isRunning) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            default:
                game.handleEvent(e);
            }
        }

        game.update();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        game.draw(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}
