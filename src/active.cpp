#include "active.h"
#include "array"

std::array<std::array<int, 2>, 2> test = {{{0, 0}, {0, 0}}};

// clang-format off
inline const std::array<std::array<std::array<uint8_t, 4>, 4>, 7> TETROMINOS = {{
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
// clang-format on

Active::Active(uint8_t type) {
    respawn(type);
}

void Active::respawn(uint8_t type) {
    // Set new Tetromino type
    m_type = type;
    // Load corresponding Tetromino into grid
    loadTetromino();
    // Set starting position
    m_x = STARTING_POSITION_X;
    m_y = STARTING_POSITION_Y;
}

void Active::loadTetromino() {
    m_grid = TETROMINOS[m_type];
}