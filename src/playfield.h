#include "SDL.h"

inline const int CELL_SIZE = 20;
inline const int GRID_SIZE_X = 10;
inline const int GRID_SIZE_Y = 40;
inline const int WINDOW_X = GRID_SIZE_X * CELL_SIZE;
inline const int WINDOW_Y = GRID_SIZE_Y * CELL_SIZE;
inline const int GRID_SIZE_VISIBLE_X = GRID_SIZE_X;
inline const int GRID_SIZE_VISIBLE_Y = 20;
inline const int GRID_START_Y = GRID_SIZE_Y - GRID_SIZE_VISIBLE_Y;

inline const uint8_t GRID_COLOR[3] = {200, 200, 200};
inline const uint8_t COLORS[][3] = {
    {255, 255, 255}, // White
    {0, 255, 255},   // I: Cyan
    {255, 255, 0}    // O: Yellow
};

class Playfield {
    uint8_t grid[GRID_SIZE_Y][GRID_SIZE_X];
    void drawGrid(SDL_Renderer *renderer);

  public:
    Playfield();
    void draw(SDL_Renderer *renderer, int pos_x, int pos_y);
};