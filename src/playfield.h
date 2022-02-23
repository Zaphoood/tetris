#include "SDL.h"
class Playfield
{
    int grid[10][40];
    void draw(SDL_Renderer *renderer);

public:
    Playfield();
};