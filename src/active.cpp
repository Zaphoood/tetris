#include "array"
#include "iostream"
#include "stdexcept"

#include "active.h"
#include "playfield.h"

std::array<std::array<int, 2>, 2> test = {{{0, 0}, {0, 0}}};

inline const uint8_t N_TETROMINOS = 7;
// clang-format off
inline const std::array<std::array<std::array<uint8_t, 4>, 4>, N_TETROMINOS> TETROMINOS = {{
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

void Active::loadGrid() {
    m_grid = TETROMINOS[m_type];
}

void Active::respawn(uint8_t type) {
    // Move the Tetromino to the top and load the new Tetromino
    // into the grid
    if (type < 0 || type >= N_TETROMINOS) {
        throw std::out_of_range("Tetromino type must be in range [0..6].");
    }
    // Set new Tetromino type
    m_type = type;
    // Load corresponding Tetromino into grid
    loadGrid();
    // Set starting position
    m_x = STARTING_POSITION_X;
    m_y = STARTING_POSITION_Y;
}

void Active::setAndRespawn(uint8_t new_type, Playfield *playfield) {
    // 'Bake' the current Tetromino into the Playfield, afterwards respawn
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (m_grid[y][x]) {
                playfield->setAt(m_x + x, m_y + y, m_type);
            }
        }
    }
    respawn(new_type);
}

void Active::draw(SDL_Renderer *renderer, Playfield *playfield) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (m_grid[y][x]) {
                playfield->drawMino(renderer, m_x + x, m_y + y,
                                    TETROMINO_COLORS[m_type]);
            }
        }
    }
}

bool Active::moveRight(Playfield *playfield) {
    bool can_move = canMoveRight(playfield);
    if (can_move) {
        m_x++;
    }
    return can_move;
}

bool Active::canMoveRight(Playfield *playfield) {
    // Get the offset of the rightmost Mino in the grid
    uint8_t rightmost = 0;
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // At right border, can't move any further
                if (m_x + col >= GRID_SIZE_X - 1 ||
                    // Spot to the right is filled
                    !playfield->isEmpty(m_x + col + 1, m_y + row)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Active::moveLeft(Playfield *playfield) {
    bool can_move = canMoveLeft(playfield);
    if (can_move) {
        m_x--;
    }
    return can_move;
}

bool Active::canMoveLeft(Playfield *playfield) {
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // At left border, can't move any further
                if (m_x + col <= 0 ||
                    // Spot to the left is filled
                    !playfield->isEmpty(m_x + col - 1, m_y + row)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Active::stepDown(Playfield *playfield) {
    if (canStepDown(playfield)) {
        m_y++;
        return true;
    } else {
        return false;
    }
}

bool Active::canStepDown(Playfield *playfield) {
    // Check if there would be any collision with a Mino on the Playfield
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // At bottom, can't move any further down
                if (m_y + row >= GRID_SIZE_Y - 1 ||
                    // Spot below is filled
                    !playfield->isEmpty(m_x + col, m_y + row + 1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::array<std::array<uint8_t, 4>, 4> Active::getGridRotatedClockw() {
    std::array<std::array<uint8_t, 4>, 4> new_grid{};
    switch (m_type) {
    case 0: // I
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                new_grid[col][3 - row] = m_grid[row][col];
            }
        }
        break;
    case 3: // O
        new_grid = m_grid;
        break;
    default: // all other Tetrominos
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                new_grid[col][2 - row] = m_grid[row][col];
            }
        }
    }
    return new_grid;
}
std::array<std::array<uint8_t, 4>, 4> Active::getGridRotatedCounterclockw() {
    std::array<std::array<uint8_t, 4>, 4> new_grid{};
    switch (m_type) {
    case 0: // I
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                new_grid[3 - col][row] = m_grid[row][col];
            }
        }
        break;
    case 3: // O
        new_grid = m_grid;
        break;
    default: // all other Tetrominos
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                new_grid[2 - col][row] = m_grid[row][col];
            }
        }
    }
    return new_grid;
}

bool Active::doesGridConflict(const std::array<std::array<uint8_t, 4>, 4> &grid,
                              Playfield *playfield) {
    // Return true if the given grid overlaps with the playfield at any point if
    // it were to replace the actual current grid
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (grid[row][col]) {
                if (!playfield->isEmpty(m_x + col, m_y + row)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Active::rotateClockw(Playfield *playfield) {
    std::array<std::array<uint8_t, 4>, 4> new_grid = getGridRotatedClockw();
    if (doesGridConflict(new_grid, playfield)) {
        // TODO: Do wall kicks!
        return false;
    } else {
        m_grid = new_grid;
        return true;
    }
}

bool Active::rotateCounterclockw(Playfield *playfield) {
    std::array<std::array<uint8_t, 4>, 4> new_grid =
        getGridRotatedCounterclockw();
    if (doesGridConflict(new_grid, playfield)) {
        // TODO: Do wall kicks!
        return false;
    } else {
        m_grid = new_grid;
        return true;
    }
}
