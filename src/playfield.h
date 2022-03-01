#pragma once
#include "array"

#include "SDL.h"

#include "constants.h"

class Playfield {
    uint8_t grid[GRID_SIZE_Y][GRID_SIZE_X];
    void drawGrid(SDL_Renderer *renderer);
    void drawPlayfield(SDL_Renderer *renderer);
    void drawActiveTetromino(SDL_Renderer *renderer);
    SDL_Rect getMinoRect(int x, int y);
    bool isRowFilled(int row);
    bool isRowEmpty(int row);
    void copyRow(int from, int to);
    void setAtHard(int x, int y, uint8_t mino_type);

  public:
    Playfield();

    uint8_t getAt(int x, int y);
    bool isEmpty(int x, int y);
    bool setAt(int x, int y, uint8_t mino_type);
    void clearAt(int x, int y);
    void clearEmptyLines();

    void drawMino(SDL_Renderer *renderer, int x, int y,
                  const std::array<uint8_t, 3> &color);
    void drawGhostMino(SDL_Renderer *renderer, int x, int y);
    void draw(SDL_Renderer *renderer, int pos_x, int pos_y);
};