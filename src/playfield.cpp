#include "playfield.h"
#include "SDL.h"

Playfield::Playfield() {}

void Playfield::draw(SDL_Renderer *renderer, int pos_x, int pos_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int row = 0; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            SDL_Rect rect{col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE,
                          CELL_SIZE};
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}