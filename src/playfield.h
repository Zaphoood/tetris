#pragma once
#include "SDL.h"
#include "array"

inline const int CELL_SIZE = 20;
inline const int GRID_SIZE_X = 10;
inline const int GRID_SIZE_Y = 40;
inline const int WINDOW_X = GRID_SIZE_X * CELL_SIZE;
inline const int WINDOW_Y = GRID_SIZE_Y * CELL_SIZE;
inline const int GRID_SIZE_VISIBLE_X = GRID_SIZE_X;
inline const int GRID_SIZE_VISIBLE_Y = 20;
inline const int GRID_START_Y = GRID_SIZE_Y - GRID_SIZE_VISIBLE_Y;

inline const uint8_t GRID_COLOR[3] = {200, 200, 200};
inline const std::array<uint8_t, 3> BACKGROUND = {{255, 255, 255}};
inline const std::array<std::array<uint8_t, 3>, 7> TETROMINO_COLORS = {{
    {{0, 255, 255}}, // I: Cyan
    {{255, 255, 0}}, // O: Yellow
    {{139, 0, 139}}, // T: Purple
    {{0, 255, 0}},   // S: Green
    {{255, 0, 0}},   // Z: Red
    {{0, 0, 255}},   // J: Blue
    {{255, 165, 0}}  // L: Orange
}};

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