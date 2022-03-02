#include "tetrovis.h"

TetroVisual::TetroVisual(TetrominoKind_t kind)
    : m_kind(kind), m_color(TETROMINO_COLORS[m_kind]) {
    loadGrid();
}

void TetroVisual::loadGrid() {
    m_grid = TETROMINOS[m_kind];
}

void TetroVisual::draw(SDL_Renderer *renderer, Playfield *p_playfield, int x,
                       int y) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                drawMino(renderer, p_playfield, x, y);
            }
        }
    }
}

void TetroVisual::drawMino(SDL_Renderer *renderer, Playfield *p_playfield,
                           int x, int y) {
    p_playfield->drawMino(renderer, x, y, m_color);
}

void TetroGhostVisual::drawMino(SDL_Renderer *renderer, Playfield *p_playfield,
                                int x, int y) {
    p_playfield->drawGhostMino(renderer, x, y);
}