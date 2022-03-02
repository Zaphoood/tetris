#include "array"
#include "iostream"
#include "stdint.h"

#include "SDL.h"

#include "active.h"
#include "playfield.h"

Playfield::Playfield() {
    // Initialize all cells to 7, which represents an empty space
    // values 0~6 correspond to different Minos
    std::cout << "Playfield initialized\n";
    for (int row = 0; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            grid[row][col] = 7;
        }
    }
}

void Playfield::draw(SDL_Renderer *renderer, int pos_x, int pos_y) {
    // TODO: Accept different drawing positions
    drawGrid(renderer);
    drawPlayfield(renderer);
}

void Playfield::drawPlayfield(SDL_Renderer *renderer) {
    for (int row = GRID_START_Y; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            if (grid[row][col] < 7) {
                drawMino(renderer, col, row, TETROMINO_COLORS[grid[row][col]]);
            }
        }
    }
}

void Playfield::drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, GRID_COLOR[0], GRID_COLOR[1],
                           GRID_COLOR[2], 255);
    // Draw horizontal lines
    for (int y = GRID_SIZE_VISIBLE_Y - 1; y > 0; y--) {
        SDL_RenderDrawLine(renderer, 0, y * CELL_SIZE, WINDOW_X, y * CELL_SIZE);
    }
    // Draw vertical lines
    for (int x = GRID_SIZE_X - 1; x > 0; x--) {
        SDL_RenderDrawLine(renderer, x * CELL_SIZE, 0, x * CELL_SIZE, WINDOW_Y);
    }
}

SDL_Rect Playfield::getMinoRect(int x, int y) {
    // Compute rectangle according to cell size
    return SDL_Rect{x * CELL_SIZE, (y - GRID_START_Y) * CELL_SIZE, CELL_SIZE,
                    CELL_SIZE};
}

void Playfield::drawMino(SDL_Renderer *renderer, int x, int y,
                         const std::array<uint8_t, 3> &color) {
    SDL_Rect rect = getMinoRect(x, y);
    // Draw a square with the color according to the current Mino
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    SDL_RenderFillRect(renderer, &rect);
    // Draw a black outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Playfield::drawGhostMino(SDL_Renderer *renderer, int x, int y) {
    // Draw a grey outline
    SDL_SetRenderDrawColor(renderer, GHOST_COLOR[0], GHOST_COLOR[1],
                           GHOST_COLOR[2], 255);
    SDL_Rect rect = getMinoRect(x, y);
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
    std::array<bool, GRID_SIZE_Y> cleared_lines{};
    for (int row = GRID_SIZE_VISIBLE_Y; row < GRID_SIZE_Y; row++) {
        if (isRowFilled(row)) {
            for (int col = 0; col < GRID_SIZE_X; col++) {
                clearAt(col, row);
            }
            cleared_lines[row] = 1;
        }
    }
    // Iterate over the columns from bottom to top. Everytime we encounter a
    // line that's just been cleared, copy down everything from above
    // TODO: Should be GRID_SIZE_Y - GRID_SIZE_VISIBLE_Y instead of
    // GRID_SIZE_VISIBLE_Y
    for (int row = GRID_SIZE_Y - 1; row >= GRID_SIZE_VISIBLE_Y; row--) {
        if (cleared_lines[row]) {
            for (int row_i = row; row_i > GRID_SIZE_VISIBLE_Y; row_i--) {
                // Copy row above into current row
                copyRow(row_i - 1, row_i);
                // Same for cleared lines
                cleared_lines[row_i] = cleared_lines[row_i - 1];
            }
            // Check this row again in the next iteration
            row++;
        }
    }
}