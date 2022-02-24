#include "playfield.h"
#include "SDL.h"
#include "iostream"

Playfield::Playfield() {
    // Initialize all cells to zero
    for (int i = 0; i < GRID_SIZE_Y; i++) {
        for (int j = 0; j < GRID_SIZE_X; j++) {
            grid[i][j] = 0;
        }
    }

    // Set bottom right cell to 1
    grid[39][0] = 1;
}

void Playfield::draw(SDL_Renderer *renderer, int pos_x, int pos_y) {
    drawGrid(renderer);
    for (int row = GRID_START_Y; row < GRID_SIZE_Y; row++) {
        for (int col = 0; col < GRID_SIZE_X; col++) {
            SDL_Rect rect{col * CELL_SIZE, (row - GRID_START_Y) * CELL_SIZE,
                          CELL_SIZE, CELL_SIZE};
            if (grid[row][col] > 0) {
                // Draw a square with the color according to the current Mino
                SDL_SetRenderDrawColor(renderer, COLORS[grid[row][col]][0],
                                       COLORS[grid[row][col]][1],
                                       COLORS[grid[row][col]][2], 255);
                SDL_RenderFillRect(renderer, &rect);
                // Draw a black outline
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
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