#include "playfield.h"
#include "SDL.h"
#include "active.h"
#include "array"
#include "iostream"
#include "stdint.h"

Playfield::Playfield() {
    // Initialize all cells to 7, which represents an empty space
    // values 0~6 correspond to different Minos
    for (int i = 0; i < GRID_SIZE_Y; i++) {
        for (int j = 0; j < GRID_SIZE_X; j++) {
            grid[i][j] = 7;
        }
    }

    // Set bottom right cell to 0
    grid[39][0] = 0;
}

void Playfield::draw(SDL_Renderer *renderer, Active *active, int pos_x,
                     int pos_y) {
    // TODO: Accept different drawing positions
    drawGrid(renderer);
    drawPlayfield(renderer);
    drawActiveTetromino(renderer, active);
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

void Playfield::drawActiveTetromino(SDL_Renderer *renderer, Active *active) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            switch (active->m_grid[y][x]) {
            case 0:
                break;
            case 1:
                drawMino(renderer, active->m_x + x, active->m_y + y,
                         TETROMINO_COLORS[active->m_type]);
                break;
            default:
                break;
            }
        }
    }
}

void Playfield::drawMino(SDL_Renderer *renderer, int x, int y,
                         const std::array<uint8_t, 3> &color) {
    // Compute rectangle according to cell size
    SDL_Rect rect{x * CELL_SIZE, (y - GRID_START_Y) * CELL_SIZE, CELL_SIZE,
                  CELL_SIZE};
    // Draw a square with the color according to the current Mino
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    SDL_RenderFillRect(renderer, &rect);
    // Draw a black outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}
