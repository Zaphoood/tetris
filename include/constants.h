#pragma once
#include "array"
#include "stdint.h"

#include "SDL.h"

// Input
inline const int KEY_INIT_DELAY_MS = 500;
inline const int KEY_REPEAT_DELAY_MS = 50;

// Framerate
inline const int MIN_FRAMETIME_MS = 15;

// Tetromino
// Define a type that stores a kind of Tetromino, such as O or L
using TetrominoKind_t = uint8_t;
// TODO: Apply this type defintion everywhere
// Number of different Tetrominos
inline const uint8_t N_TETROMINOS = 7;

// Tetromino starting position
inline const int STARTING_POSITION_X = 3;
inline const int STARTING_POSITION_Y = 18;

// Tetromino fall delay
inline const int FALL_DELAY_MS = 250;
// Soft dropping is 20 times faster than normal falling, therefore the delay is
// multiplied by 0.05
inline const double SOFT_DROP_DELAY_MULT = 0.05;
inline const int LOCK_DOWN_DELAY_MS = 500;

// Tetromino generation
inline const int QUEUE_LEN = 3;

// Playfield
inline const int GRID_SIZE_X = 10;
inline const int GRID_SIZE_Y = 40;
// Visible portion of the grid; for width this is redundant, but regarding the
// height it's very important since only half of the actual grid is show
inline const int GRID_SIZE_VISIBLE_X = GRID_SIZE_X;
inline const int GRID_SIZE_VISIBLE_Y = 20;
// Where the visible part of the grid starts
inline const int GRID_START_Y = GRID_SIZE_Y - GRID_SIZE_VISIBLE_Y;
// Size of a cell in pixels
inline const int CELL_SIZE = 40;
// Size of the playfield on screen in pixels
inline const int PLAYFIELD_WIDTH = GRID_SIZE_X * CELL_SIZE;
inline const int PLAYFIELD_HEIGHT = GRID_SIZE_VISIBLE_Y * CELL_SIZE;

// Game
enum class GameState { PreInit, Running, Paused, GameOver };

// Window
// Include enough space to the right to show queue
inline const int WINDOW_X = PLAYFIELD_WIDTH + 2 * (CELL_SIZE * 7);
inline const int WINDOW_Y = PLAYFIELD_HEIGHT + 1;

// Position of the playfield on screen in pixels
inline const int PLAYFIELD_DRAW_X = (WINDOW_X - PLAYFIELD_WIDTH) / 2;
inline const int PLAYFIELD_DRAW_Y = 0;

// Queue
inline const int QUEUE_X = PLAYFIELD_DRAW_X + PLAYFIELD_WIDTH + CELL_SIZE;
inline const int QUEUE_Y = CELL_SIZE * 2;
inline const int QUEUE_OFFSET_Y = CELL_SIZE * 5;

// Hold Tetromino
inline const int HOLD_X = PLAYFIELD_DRAW_X - (int)(CELL_SIZE * 5.5);
inline const int HOLD_Y = QUEUE_Y;

// Colors
inline const SDL_Color GRID_COLOR{152, 139, 162, 0};
inline const SDL_Color BACKGROUND{48, 45, 65, 0};
inline const SDL_Color GHOST_COLOR{152, 139, 163, 0};
inline const SDL_Color TEXT_COLOR{217, 224, 238, 0};
inline const std::array<SDL_Color, 7> TETROMINO_COLORS = {{
    {150, 205, 251, 0}, // I: Cyan
    {250, 227, 176, 0}, // O: Yellow
    {221, 182, 242, 0}, // T: Purple
    {171, 232, 224, 0}, // S: Green
    {242, 143, 173, 0}, // Z: Red
    {150, 205, 251, 0}, // J: Blue
    {248, 189, 150, 0}  // L: Orange
}};

// Text
inline const char FONT_PATH_RELATIVE[] = "/futura-medium.ttf";
inline const float FONT_SIZE_SCALE = 0.9;
inline const int FONT_SIZE = (int)(CELL_SIZE * FONT_SIZE_SCALE);
inline const int LINE_OFFSET = (int)(50 * FONT_SIZE_SCALE);
inline const int INFO_TEXT_X = 30;
inline const int INFO_TEXT_Y = HOLD_Y + 4 * CELL_SIZE; // + LINE_OFFSET;

inline const int SCORE_TEXT_X = INFO_TEXT_X;
inline const int SCORE_TEXT_Y = INFO_TEXT_Y;

inline const int LEVEL_TEXT_X = INFO_TEXT_X;
inline const int LEVEL_TEXT_Y = SCORE_TEXT_Y + (int)(2 * LINE_OFFSET);
inline const int GOAL_TEXT_X = INFO_TEXT_X;
inline const int GOAL_TEXT_Y = LEVEL_TEXT_Y + LINE_OFFSET;
inline const int LINES_TEXT_X = INFO_TEXT_X;
inline const int LINES_TEXT_Y = GOAL_TEXT_Y + LINE_OFFSET;

// x-position of 'Paused' text is calculated dynamically
inline const int PAUSED_TEXT_Y = PLAYFIELD_DRAW_Y + PLAYFIELD_HEIGHT * 0.45;

// Scoring
inline const int LINES_PER_LEVEL = 10;
inline const std::array<int, 4> LINE_CLEAR_REWARD = {{100, 300, 500, 800}};
inline const std::array<int, 4> MINI_T_SPIN_REWARD = {{100, 200}};
inline const std::array<int, 4> T_SPIN_REWARD = {{400, 800, 1200, 1600}};

// Four-by-four Matrix containing the grid representation of a Tetromino
using TetroGrid_t = std::array<std::array<uint8_t, 4>, 4>;
// A single Wall Kick
using Wallkick_t = std::array<int8_t, 2>;
// Set of all 5 Wall Kicks to try for a combination of Tetromino type and
// direction of rotation
using WallkickData_t = std::array<Wallkick_t, 5>;

// Tetromino grid representations
// clang-format off
inline const std::array<TetroGrid_t, N_TETROMINOS> TETROMINOS = {{
    // I
    {{
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }},
    // J
    {{
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
    // L
    {{
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
    // O
    {{
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
    // S
    {{
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
    // T
    {{
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
    // Z
    {{
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    }},
}};

// Wallkick data
// ... for clockwise rotation of I Tetromino
inline const std::array<WallkickData_t, 4> WALLKICK_I_C =
{{
    {{{{ 0,   0}}, {{-2,  0}}, {{ 1,  0}}, {{-2, -1}}, {{ 1,  2}}}}, // Starting from orienation=0
    {{{{ 0,   0}}, {{-1,  0}}, {{ 2,  0}}, {{-1, -2}}, {{ 2,  1}}}}, // Starting from orienation=1
    {{{{ 0,   0}}, {{ 2,  0}}, {{-1,  0}}, {{ 2, -1}}, {{-1,  2}}}}, // Starting from orienation=2
    {{{{ 0,   0}}, {{ 1,  0}}, {{-2,  0}}, {{ 1,  2}}, {{-2, -1}}}}, // Starting from orienation=3
}};

// ... for clockwise rotation of all other Tetromino types except O
inline const std::array<WallkickData_t, 4> WALLKICK_OTHER_C = 
{{
    {{{{ 0,   0}}, {{-1,  0}}, {{-1, -1}}, {{ 0,  2}}, {{-1,  2}}}}, // Starting from orienation=0
    {{{{ 0,   0}}, {{ 1,  0}}, {{ 1,  1}}, {{ 0, -2}}, {{ 1, -2}}}}, // Starting from orienation=1
    {{{{ 0,   0}}, {{ 1,  0}}, {{ 1, -1}}, {{ 0,  2}}, {{ 1,  2}}}}, // Starting from orienation=2
    {{{{ 0,   0}}, {{-1,  0}}, {{-1,  1}}, {{ 0, -2}}, {{-1, -2}}}}, // Starting from orienation=3
}};

// ... for counterclockwise rotation of I Tetromino
inline const std::array<WallkickData_t, 4> WALLKICK_I_CC =
{{
    {{{{ 0,   0}}, {{-1,  0}}, {{ 2,  0}}, {{-1, -2}}, {{ 2,  1}}}}, // Starting from orienation=0
    {{{{ 0,   0}}, {{ 2,  0}}, {{-1,  0}}, {{ 2, -1}}, {{-1,  2}}}}, // Starting from orienation=1
    {{{{ 0,   0}}, {{ 1,  0}}, {{-2,  0}}, {{ 1,  2}}, {{-2, -1}}}}, // Starting from orienation=2
    {{{{ 0,   0}}, {{-2,  0}}, {{ 1,  0}}, {{-2,  1}}, {{ 1, -2}}}}, // Starting from orienation=3
}};

// ... for counterclockwise rotation of all other Tetromino types types except O
inline const std::array<WallkickData_t, 4> WALLKICK_OTHER_CC = 
{{
    {{{{ 0,   0}}, {{ 1,  0}}, {{ 1, -1}}, {{ 0,  2}}, {{ 1,  2}}}}, // Starting from orienation=0
    {{{{ 0,   0}}, {{ 1,  0}}, {{ 1,  1}}, {{ 0, -2}}, {{ 1, -2}}}}, // Starting from orienation=1
    {{{{ 0,   0}}, {{-1,  0}}, {{-1, -1}}, {{ 0,  2}}, {{-1,  2}}}}, // Starting from orienation=2
    {{{{ 0,   0}}, {{-1,  0}}, {{-1,  1}}, {{ 0, -2}}, {{-1, -2}}}}, // Starting from orienation=3
}};
// clang-format on
