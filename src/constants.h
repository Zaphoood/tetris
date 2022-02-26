#pragma once
#include "array"
#include "stdint.h"

// Tetromino
// Number of different Tetrominos
inline const uint8_t N_TETROMINOS = 7;

// Tetromino starting position
inline const int STARTING_POSITION_X = 3;
inline const int STARTING_POSITION_Y = 18;

// Tetromino fall delay
inline const int FALL_DELAY_MS = 250;

// Playfield
inline const int CELL_SIZE = 20;
inline const int GRID_SIZE_X = 10;
inline const int GRID_SIZE_Y = 40;
inline const int GRID_SIZE_VISIBLE_X = GRID_SIZE_X;
inline const int GRID_SIZE_VISIBLE_Y = 20;
inline const int GRID_START_Y = GRID_SIZE_Y - GRID_SIZE_VISIBLE_Y;

// Window
inline const int WINDOW_X = GRID_SIZE_X * CELL_SIZE;
inline const int WINDOW_Y = GRID_SIZE_Y * CELL_SIZE;

// Colors
using ColorRGB = std::array<uint8_t, 3>;
inline const ColorRGB GRID_COLOR = {{200, 200, 200}};
inline const ColorRGB BACKGROUND = {{255, 255, 255}};
// inline const ColorRGB GHOST_COLOR = {{100, 100, 100}};
inline const ColorRGB GHOST_COLOR = {{100, 100, 100}};
inline const std::array<ColorRGB, 7> TETROMINO_COLORS = {{
    {{0, 255, 255}}, // I: Cyan
    {{255, 255, 0}}, // O: Yellow
    {{139, 0, 139}}, // T: Purple
    {{0, 255, 0}},   // S: Green
    {{255, 0, 0}},   // Z: Red
    {{0, 0, 255}},   // J: Blue
    {{255, 165, 0}}  // L: Orange
}};