#include "array"
#include "iostream"
#include "stdexcept"

#include "active.h"
#include "constants.h"
#include "playfield.h"

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

Active::Active() : m_type(-1) {}
Active::Active(uint8_t type) {
    respawn(type);
}

void Active::loadGrid() {
    m_grid = TETROMINOS[m_type];
}

void Active::respawn(uint8_t type) {
    // Move the Tetromino to the top and load the new Tetromino
    // into m_grid
    if (type < 0 || type >= N_TETROMINOS) {
        throw std::out_of_range("Tetromino type must be in range [0..6].");
    }
    // Set new Tetromino type
    m_type = type;
    // Reset the orientation
    m_orientation = 0;
    // Load corresponding Tetromino into grid
    loadGrid();
    // Set starting position
    m_x = STARTING_POSITION_X;
    m_y = STARTING_POSITION_Y;
}

void Active::lockDownAndRespawn(uint8_t new_type, Playfield *playfield) {
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

int Active::getGhostY(Playfield *playfield) {
    // Return the Y position of the Ghost Tetromino.

    int ghost_y = GRID_SIZE_Y;
    // Iterate over all columns of the Tetromino and see which has the least
    // space below until the next Mino on the Playfield, calculate new position
    // to be 1 cell above that
    for (int col = 0; col < 4; col++) {
        // Vertical position of the lowest Mino contained in the current column
        // of the Tetromino, relative to the postition of the Tetromino
        int lowest_mino_rel = -1;
        for (int row = 0; row < 4; row++) {
            if (m_grid[row][col]) {
                lowest_mino_rel = row;
            }
        }
        if (lowest_mino_rel == -1) {
            // No Mino in current column -> skip
            continue;
        }
        // Lowest position for each column: at the very bottom
        ghost_y = std::min(ghost_y, GRID_SIZE_Y - lowest_mino_rel - 1);
        // Check the corresponding Playfield column and get position of lowest
        // free cell below.
        // Absolute position of the lowest Mino in the current column
        int lowest_mino = m_y + lowest_mino_rel;
        for (int cell_y = lowest_mino + 1; cell_y < GRID_SIZE_Y; cell_y++) {
            if (!(*playfield).isEmpty(m_x + col, cell_y)) {
                ghost_y = std::min(ghost_y, cell_y - lowest_mino_rel - 1);
            }
        }
    }
    return ghost_y;
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
    // Check if already at the bottom or if there would be any collision with a
    // Mino on the Playfield
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

void Active::hardDrop(Playfield *playfield) {
    // Hard drop the falling Tetromino, i. e. move it down as far as possible
    // Note that this doesn't lock nor respawn the Tetromino!

    m_y = getGhostY(playfield);
}

TetroGrid_t Active::getGridRotatedClockw() {
    // Return a new grid that is the same as the current one, but the contents
    // are rotated clockwise

    TetroGrid_t new_grid{};
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

TetroGrid_t Active::getGridRotatedCounterclockw() {
    // Return a new grid that is the same as the current one, but the contents
    // are rotated counterclockwise
    TetroGrid_t new_grid{};
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

bool Active::doesOffsetGridConflict(const TetroGrid_t &grid, int dx, int dy,
                                    Playfield *playfield) {
    // Return true if the given grid overlaps with the playfield at any cell if
    // it were to replace the actual current grid and be offset by the given
    // amount
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (grid[row][col]) {
                if (!playfield->isEmpty(m_x + col + dx, m_y + row + dy) ||
                    m_x + col + dx < 0 || m_x + col + dx >= GRID_SIZE_X ||
                    m_y + col + dy < 0 || m_y + col + dy >= GRID_SIZE_Y) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Active::doesGridConflict(const TetroGrid_t &grid, Playfield *playfield) {
    // Return true if the given grid overlaps with the playfield at any cell if
    // it were to replace the actual current grid
    return doesOffsetGridConflict(grid, 0, 0, playfield);
}

bool Active::tryWallkicksC(const TetroGrid_t &new_grid, Wallkick_t &success,
                           Playfield *playfield) {
    // Try Wall Kicks for clockwise rotation
    return tryWallkicks(new_grid, 1, success, playfield);
}

bool Active::tryWallkicksCC(const TetroGrid_t &new_grid, Wallkick_t &success,
                            Playfield *playfield) {
    // Try Wall Kicks for counterclockwise rotation
    return tryWallkicks(new_grid, -1, success, playfield);
}

bool Active::tryWallkicks(const TetroGrid_t &new_grid, int8_t direction,
                          Wallkick_t &success, Playfield *playfield) {
    // Try all Wall Kicks for the given grid and direction of rotation. If a
    // working Wall Kick is found, it is stored in &success and true is
    // returned; otherwise false.

    // O Tetromino; doesn't perform Wall Kicks
    if (m_type == 3) {
        success = Wallkick_t{0, 0};
        return true;
    }
    // Load the right wallkick data depending on the Tetromino type and
    // direction of rotation
    const WallkickData_t *wallkick_data = nullptr;
    if (direction > 0) {   // Clockwise rotation
        if (m_type == 0) { // I Tetromino
            wallkick_data = &WALLKICK_I_C[m_orientation];
        } else { // All other Tetrominos
            wallkick_data = &WALLKICK_OTHER_C[m_orientation];
        }
    } else {               // Counterclockwise rotation
        if (m_type == 0) { // I Tetromino
            wallkick_data = &WALLKICK_I_CC[m_orientation];
        } else { // All other Tetrominos
            wallkick_data = &WALLKICK_OTHER_CC[m_orientation];
        }
    }
    // Try out all Wall Kicks
    return tryWallkickData(new_grid, wallkick_data, success, playfield);
}

bool Active::tryWallkickData(const TetroGrid_t &new_grid,
                             const WallkickData_t *wallkick_data,
                             Wallkick_t &success, Playfield *playfield) {
    // Iterate over all given Wall Kicks to see if one works
    for (uint8_t i = 0; i < wallkick_data->size(); i++) {
        // Check if there would be a conflict using the current Wall Kick
        if (!doesOffsetGridConflict(new_grid, (*wallkick_data)[i][0],
                                    (*wallkick_data)[i][1], playfield)) {
            // Possible Wall Kick found
            success = (*wallkick_data)[i];
            return true;
        }
    }
    // No Wall Kick found
    return false;
}

bool Active::rotateClockw(Playfield *playfield) {
    // If possible, perform a clockwise rotation

    // Generate rotated grid
    TetroGrid_t new_grid = getGridRotatedClockw();
    // Try to perform Wall Kick. Note that no offset (i. e. [0, 0]) is the first
    // Wall Kick that is tried first, therefore it isn't necesarry to
    // exclusively check if the new grid fits without a wall kick.
    Wallkick_t wallkick;
    if (!tryWallkicksC(new_grid, wallkick, playfield)) {
        // No Wall Kick found -> rotation is impossible
        return false;
    }
    // Wall Kick found, apply it
    m_x += wallkick[0];
    m_y += wallkick[1];
    // Replace old grid with new, rotated grid
    m_grid = new_grid;
    // Change rotation accordingly
    m_orientation = (m_orientation + 1) % 4;
    return true;
}

bool Active::rotateCounterclockw(Playfield *playfield) {
    // If possible, perform a counterclockwise rotation

    TetroGrid_t new_grid = getGridRotatedCounterclockw();
    Wallkick_t wallkick;
    if (!tryWallkicksCC(new_grid, wallkick, playfield)) {
        // No Wall Kick found -> rotation is impossible
        return false;
    }
    // Wall Kick found, apply it
    m_x += wallkick[0];
    m_y += wallkick[1];
    // Replace old grid with new, rotated grid
    m_grid = new_grid;
    // We can't use (m_orientation - 1) here since that might overflow to
    // 255. Adding 3 works just fine tho since 3 ≡ -1 (mod 4)
    m_orientation = (m_orientation + 3) % 4;
    return true;
}

void Active::draw(SDL_Renderer *renderer, Playfield *playfield) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                playfield->drawMino(renderer, m_x + col, m_y + row,
                                    TETROMINO_COLORS[m_type]);
            }
        }
    }
}

void Active::drawGhost(SDL_Renderer *renderer, Playfield *playfield) {
    int ghost_y = getGhostY(playfield);
    //  Don't draw the Ghost if it's at the same position as the actual
    //  Tetromino
    if (ghost_y > m_y) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (m_grid[row][col]) {
                    playfield->drawGhostMino(renderer, m_x + col,
                                             ghost_y + row);
                }
            }
        }
    }
}