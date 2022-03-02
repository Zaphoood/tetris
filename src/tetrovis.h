#include "SDL.h"

#include "constants.h"
#include "playfield.h"

class TetroVisual {
  private:
    TetrominoKind_t m_kind;
    TetroGrid_t m_grid;
    ColorRGB m_color;
    void drawMino(SDL_Renderer *renderer, Playfield *p_playfield, int x, int y);

  public:
    TetroVisual(TetrominoKind_t m_kind);
    void loadGrid();
    void draw(SDL_Renderer *renderer, Playfield *playfield, int x, int y);
};

class TetroGhostVisual : public TetroVisual {
  private:
    void drawMino(SDL_Renderer *renderer, Playfield *p_playfield, int x, int y);
};