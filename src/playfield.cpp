#include "array"
#include "constants.h"
#include "iostream"
#include "stdint.h"

#include "SDL.h"

#include "active.h"
#include "playfield.h"

Playfield::Playfield() : Playfield(0, 0) {}
Playfield::Playfield(int draw_x, int draw_y)
    : m_draw_x(draw_x), m_draw_y(draw_y) {
    reset();
}

void Playfield::reset() {
    // Initialize all cells to 7, which represents an empty space
    // values 0~6 correspond to different Minos
    for (int row = 0; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            m_grid[row][col] = 7;
        }
    }
}

std::array<int, 2> Playfield::cellToPixelPosition(int cell_x, int cell_y) {
    return std::array<int, 2>{m_draw_x + cell_x * CELL_SIZE,
                              m_draw_y + (cell_y - GRID_START_Y) * CELL_SIZE};
}

void Playfield::draw(SDL_Renderer *renderer) {
    // TODO: Accept different drawing positions
    drawOutline(renderer);
    drawPlayfield(renderer);
}

void Playfield::setDrawPosition(int x, int y) {
    m_draw_x = x;
    m_draw_y = y;
}

void Playfield::drawPlayfield(SDL_Renderer *renderer) {
    std::array<int, 2> pos;
    for (int row = GRID_START_Y; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            if (m_grid[row][col] < 7) {
                pos = cellToPixelPosition(col, row);
                drawMino(renderer, pos[0], pos[1],
                         TETROMINO_COLORS[m_grid[row][col]]);
            }
        }
    }
}

void Playfield::drawOutline(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b,
                           GRID_COLOR.a);
    // clang-format off
    // Draw vertical lines
    SDL_RenderDrawLine(renderer,
        m_draw_x,                   m_draw_y,
        m_draw_x,                   m_draw_y + PLAYFIELD_HEIGHT);
    SDL_RenderDrawLine(renderer,
        m_draw_x + PLAYFIELD_WIDTH, m_draw_y,
        m_draw_x + PLAYFIELD_WIDTH, m_draw_y + PLAYFIELD_HEIGHT);
    // Draw horizontal lines
    SDL_RenderDrawLine(renderer,
        m_draw_x,                   m_draw_y,
        m_draw_x + PLAYFIELD_WIDTH, m_draw_y);
    SDL_RenderDrawLine(renderer,
        m_draw_x,                   m_draw_y + PLAYFIELD_HEIGHT,
        m_draw_x + PLAYFIELD_WIDTH, m_draw_y + PLAYFIELD_HEIGHT);
    // clang-format on
}

void Playfield::drawMino(SDL_Renderer *renderer, int x, int y,
                         const SDL_Color &color) {
    // Draw a Mino at the given pixel position

    // Create destination rectangle
    SDL_Rect rect{x, y, CELL_SIZE + 1, CELL_SIZE + 1};
    // Draw a rectangle filled with the given color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.b);
    SDL_RenderFillRect(renderer, &rect);
    // Draw a black outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    // There seems to be a bug with SDL_RenderDrawRect since the bottom-right
    // pixel of a rect sometimes isn't drawn
    SDL_RenderDrawPoint(renderer, rect.x + rect.w - 1, rect.y + rect.h - 1);
}

void Playfield::drawGhostMino(SDL_Renderer *renderer, int x, int y) {
    // Draw a Mino representing a Ghost Piece at the given pixel position

    // Create destination rectangle
    SDL_Rect rect = {x, y, CELL_SIZE + 1, CELL_SIZE + 1};
    // Draw a grey outline
    SDL_SetRenderDrawColor(renderer, GHOST_COLOR.r, GHOST_COLOR.g,
                           GHOST_COLOR.b, GHOST_COLOR.a);
    SDL_RenderDrawRect(renderer, &rect);
}

uint8_t Playfield::getAt(int x, int y) {
    return m_grid[y][x];
}

bool Playfield::isObstructed(int x, int y) {
    if (x < 0 || x >= GRID_SIZE_X || y < 0 || y >= GRID_SIZE_Y) {
        return true;
    }
    TetrominoKind_t val = getAt(x, y);
    return 0 <= val && val < 7;
}

bool Playfield::setAt(int x, int y, uint8_t mino_type) {
    // Only set the cell if mino_type is valid
    if (mino_type < 0 || mino_type > 6) {
        return false;
    } else {
        setAtHard(x, y, mino_type);
        return true;
    }
}

void Playfield::setAtHard(int x, int y, uint8_t mino_type) {
    m_grid[y][x] = mino_type;
}

void Playfield::clearAt(int x, int y) {
    m_grid[y][x] = 7;
}

bool Playfield::isRowFilled(int row) {
    for (int col = 0; col < GRID_SIZE_X; col++) {
        if (!isObstructed(col, row)) {
            return false;
        }
    }
    return true;
}

void Playfield::copyRow(int from, int to) {
    for (int col = 0; col < GRID_SIZE_X; col++) {
        setAtHard(col, to, getAt(col, from));
    }
}

/**
 * Clear empty lines from the playfield
 *
 * @return the number of removed lines
 */
int Playfield::clearEmptyLines() {
    // Only store cleared lines for the visible portion of the grid (which
    // causes all those 'GRID_START_Y + ...' offsets)
    std::array<bool, GRID_SIZE_VISIBLE_Y> cleared_lines{};
    int n_cleared = 0;
    for (int row = 0; row < GRID_SIZE_VISIBLE_Y; row++) {
        if (isRowFilled(GRID_START_Y + row)) {
            for (int col = 0; col < GRID_SIZE_X; col++) {
                clearAt(col, GRID_START_Y + row);
            }
            cleared_lines[row] = 1;
            n_cleared++;
        }
    }

    // Iterate over the columns from bottom to top. Everytime we encounter a
    // line that's just been cleared, copy down everything from above
    for (int row = GRID_SIZE_VISIBLE_Y - 1; row >= 0; row--) {
        if (cleared_lines[row]) {
            for (int row_i = row; row_i >= 0; row_i--) {
                // Copy row above into current row
                copyRow(GRID_START_Y + row_i - 1, GRID_START_Y + row_i);
                // Same for cleared lines
                cleared_lines[row_i] = cleared_lines[row_i - 1];
            }
            // Check this row again in the next iteration
            row++;
        }
    }
    return n_cleared;
}
