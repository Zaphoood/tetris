#include "tetrovis.h"

TetroVisual::TetroVisual() : m_kind(-1) {}
TetroVisual::TetroVisual(TetrominoKind_t kind) {
    setKind(kind);
}

void TetroVisual::loadGrid() {
    m_grid = TETROMINOS[m_kind];
}

void TetroVisual::setKind(TetrominoKind_t kind) {
    m_kind = kind;
    m_color = TETROMINO_COLORS[m_kind];
    loadGrid();
}

TetrominoKind_t TetroVisual::getKind() {
    return m_kind;
}

void TetroVisual::draw(SDL_Renderer *renderer, int x, int y) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                drawMino(renderer, x + col * CELL_SIZE, y + row * CELL_SIZE);
            }
        }
    }
}

<<<<<<< HEAD
void TetroVisual::drawMino(SDL_Renderer *renderer, Playfield *p_playfield,
                           int x, int y) {
    Playfield::drawMino(renderer, x, y, m_color);
}

void TetroGhostVisual::drawMino(SDL_Renderer *renderer, Playfield *p_playfield,
                                int x, int y) {
=======
void TetroVisual::drawMino(SDL_Renderer *renderer, int x, int y) {
    Playfield::drawMino(renderer, x, y, m_color);
}

void TetroGhostVisual::drawMino(SDL_Renderer *renderer, int x, int y) {
>>>>>>> Handle drawing of hud in separate class HUD
    Playfield::drawGhostMino(renderer, x, y);
}