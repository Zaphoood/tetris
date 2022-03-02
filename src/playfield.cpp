#include "array"
#include "iostream"
#include "stdint.h"

#include "SDL.h"

#include "active.h"
#include "playfield.h"

Playfield::Playfield() : Playfield(0, 0) {}
Playfield::Playfield(int draw_x, int draw_y) : draw_x(draw_x), draw_y(draw_y) {
    // Initialize all cells to 7, which represents an empty space
    // values 0~6 correspond to different Minos
    std::cout << "Playfield initialized\n";
    for (int row = 0; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            grid[row][col] = 7;
        }
    }
}

std::array<int, 2> Playfield::cellToPixelPosition(int cell_x, int cell_y) {
    return std::array<int, 2>{draw_x + cell_x * CELL_SIZE,
                              draw_y + (cell_y - GRID_START_Y) * CELL_SIZE};
}

void Playfield::draw(SDL_Renderer *renderer) {
    // TODO: Accept different drawing positions
    drawGrid(renderer);
    drawPlayfield(renderer);
}

void Playfield::setDrawPosition(int x, int y) {
    draw_x = x;
    draw_y = y;
}

void Playfield::drawPlayfield(SDL_Renderer *renderer) {
    std::array<int, 2> pos;
    for (int row = GRID_START_Y; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            if (grid[row][col] < 7) {
                pos = cellToPixelPosition(col, row);
                drawMino(renderer, pos[0], pos[1],
                         TETROMINO_COLORS[grid[row][col]]);
            }
        }
    }
}

void Playfield::drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, GRID_COLOR[0], GRID_COLOR[1],
                           GRID_COLOR[2], 255);
    // Draw horizontal lines
    for (int y = 0; y <= GRID_SIZE_VISIBLE_Y; y++) {
        // clang-format off
        SDL_RenderDrawLine(renderer,
            draw_x + 0, draw_y + y * CELL_SIZE,
            draw_x + PLAYFIELD_WIDTH, draw_y + y * CELL_SIZE);
        // clang-format on
    }
    // Draw vertical lines
    for (int x = 0; x <= GRID_SIZE_X; x++) {
        // clang-format off
        SDL_RenderDrawLine(renderer,
            draw_x + x * CELL_SIZE, draw_y,
            draw_x + x * CELL_SIZE, draw_y + PLAYFIELD_HEIGHT);
        // clang-format on
    }
}

void Playfield::drawMino(SDL_Renderer *renderer, int x, int y,
                         const ColorRGB &color) {
    // Draw a Mino at the given pixel position

    // Create destination rectangle
    SDL_Rect rect{x, y, CELL_SIZE, CELL_SIZE};
    // Draw a rectangle filled with the given color
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    SDL_RenderFillRect(renderer, &rect);
    // Draw a black outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Playfield::drawGhostMino(SDL_Renderer *renderer, int x, int y) {
    // Draw a Mino representing a Ghost Piece at the given pixel position

    // Create destination rectangle
    SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
    // Draw a grey outline
    SDL_SetRenderDrawColor(renderer, GHOST_COLOR[0], GHOST_COLOR[1],
                           GHOST_COLOR[2], 255);
    SDL_RenderDrawRect(renderer, &rect);
}

uint8_t Playfield::getAt(int x, int y) {
    return grid[y][x];
}

bool Playfield::isEmpty(int x, int y) {
    return getAt(x, y) > 6;
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
    grid[y][x] = mino_type;
}

void Playfield::clearAt(int x, int y) {
    grid[y][x] = 7;
}

bool Playfield::isRowFilled(int row) {
    for (int col = 0; col < GRID_SIZE_X; col++) {
        if (isEmpty(col, row)) {
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

void Playfield::clearEmptyLines() {
    // Only store cleared lines for the visible portion of the grid (which
    // causes all those 'GRID_START_Y + ...' offsets)
    std::array<bool, GRID_SIZE_VISIBLE_Y> cleared_lines{};
    for (int row = 0; row < GRID_SIZE_VISIBLE_Y; row++) {
        if (isRowFilled(GRID_START_Y + row)) {
            for (int col = 0; col < GRID_SIZE_X; col++) {
                clearAt(col, GRID_START_Y + row);
            }
            cleared_lines[row] = 1;
        }
    }
    // Iterate over the columns from bottom to top. Everytime we encounter a
    // line that's just been cleared, copy down everything from above
    for (int row = GRID_SIZE_VISIBLE_Y - 1; row > 0; row--) {
        if (cleared_lines[row]) {
            for (int row_i = row; row_i > 0; row_i--) {
                // Copy row above into current row
                copyRow(GRID_START_Y + row_i - 1, GRID_START_Y + row_i);
                // Same for cleared lines
                cleared_lines[row_i] = cleared_lines[row_i - 1];
            }
            // Check this row again in the next iteration
            row++;
        }
    }
}