#pragma once
#include "array"

#include "SDL.h"

#include "constants.h"

class Playfield {
    uint8_t grid[GRID_SIZE_Y][GRID_SIZE_X];
    void drawGrid(SDL_Renderer *renderer);
    void drawPlayfield(SDL_Renderer *renderer);
    void drawActiveTetromino(SDL_Renderer *renderer);

  public:
    Playfield();
    uint8_t getAt(int x, int y);
    bool setAt(int x, int y, uint8_t mino_type);
    bool isEmpty(int x, int y);
    void drawMino(SDL_Renderer *renderer, int x, int y,
                  const std::array<uint8_t, 3> &color);
    void draw(SDL_Renderer *renderer, int pos_x, int pos_y);
};