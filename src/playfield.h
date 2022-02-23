#include "SDL.h"

inline const int CELL_SIZE = 20;
inline const int GRID_SIZE_X = 10;
inline const int GRID_SIZE_Y = 40;
inline const int GRID_SIZE_VISIBLE_X = GRID_SIZE_X;
inline const int GRID_SIZE_VISIBLE_Y = 20;

class Playfield {
    int grid[GRID_SIZE_X][GRID_SIZE_Y];

  public:
    Playfield();
    void draw(SDL_Renderer *renderer, int pos_x, int pos_y);
};