#pragma once
#include "SDL.h"

#include "constants.h"
#include "playfield.h"

class TetroVisual {
  private:
    TetrominoKind_t m_kind;
    TetroGrid_t m_grid;
    SDL_Color m_color;
    void drawMino(SDL_Renderer *renderer, int x, int y);
    void loadGrid();

  public:
    TetroVisual();
    TetroVisual(TetrominoKind_t m_kind);
    void setKind(TetrominoKind_t kind);
    TetrominoKind_t getKind();
    void draw(SDL_Renderer *renderer, int x, int y);
};

class TetroGhostVisual : public TetroVisual {
  private:
    void drawMino(SDL_Renderer *renderer, int x, int y);
};
