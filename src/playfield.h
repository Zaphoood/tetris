#pragma once
#include "array"

#include "SDL.h"

#include "constants.h"

class Playfield {
    uint8_t m_grid[GRID_SIZE_Y][GRID_SIZE_X];
    int m_draw_x, m_draw_y; // Where to draw the p_playfield on the screen

    void drawOutline(SDL_Renderer *renderer);
    void drawPlayfield(SDL_Renderer *renderer);
    void drawActiveTetromino(SDL_Renderer *renderer);
    SDL_Rect getMinoRect(int x, int y);
    bool isRowFilled(int row);
    void copyRow(int from, int to);
    void setAtHard(int x, int y, uint8_t mino_type);

  public:
    Playfield();
    Playfield(int draw_x, int draw_y);

    /**
     * Reset the Playfield to its initial state
     */
    void reset();

    uint8_t getAt(int x, int y);
    bool isObstructed(int x, int y);
    bool setAt(int x, int y, uint8_t mino_type);
    void clearAt(int x, int y);
    int clearEmptyLines();

    void draw(SDL_Renderer *renderer);
    void setDrawPosition(int x, int y);

    std::array<int, 2> cellToPixelPosition(int cell_x, int cell_y);
    static void drawMino(SDL_Renderer *renderer, int x, int y,
                         const SDL_Color &color);
    static void drawGhostMino(SDL_Renderer *renderer, int x, int y);
};
