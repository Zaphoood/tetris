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

SDL_Rect Playfield::getMinoRect(int x, int y) {
    // Compute rectangle according to cell size and draw position
    // clang-format off
    return SDL_Rect{draw_x + x * CELL_SIZE,
                    draw_y + (y - GRID_START_Y) * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE};
    // clang-format on
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