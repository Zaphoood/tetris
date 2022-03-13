#include "array"
#include "iostream"
#include "stdexcept"

#include "active.h"
#include "constants.h"
#include "playfield.h"

Active::Active() : m_type(-1) {}
Active::Active(uint8_t type, Playfield *p_playfield)
    : p_playfield(p_playfield) {
    respawn(type);
}

void Active::loadGrid() {
    m_grid = TETROMINOS[m_type];
}

bool Active::respawn(uint8_t type) {
    // Load new Tetromino into the grid and move it to the respawn position;
    // return false if unsuccessfull

    if (type < 0 || type >= N_TETROMINOS) {
        throw std::out_of_range("Tetromino type must be in range [0..6].");
    }
    // Set new Tetromino type
    m_type = type;
    // Reset orientation
    m_orientation = 0;
    // Load corresponding Tetromino into grid
    loadGrid();
    // Check if respawn position is obstructed
    if (gridConflict(m_grid, STARTING_POSITION_X, STARTING_POSITION_Y)) {
        return false;
    }
    // Set starting position
    m_x = STARTING_POSITION_X;
    m_y = STARTING_POSITION_Y;
    return true;
}

void Active::lockDown() {
    // 'Bake' the current Tetromino into the playfield
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (m_grid[y][x]) {
                p_playfield->setAt(m_x + x, m_y + y, m_type);
            }
        }
    }
}

int Active::getGhostY() {
    // Return the Y position of the Ghost Tetromino.

    int ghost_y = GRID_SIZE_Y;
    // Iterate over all columns of the Tetromino and see which has the least
    // space below until the next Mino on the playfield, calculate new
    // position to be 1 cell above that
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
        // Check the corresponding playfield column and get position of lowest
        // free cell below.
        // Absolute position of the lowest Mino in the current column
        int lowest_mino = m_y + lowest_mino_rel;
        for (int cell_y = lowest_mino + 1; cell_y < GRID_SIZE_Y; cell_y++) {
            if ((*p_playfield).isObstructed(m_x + col, cell_y)) {
                ghost_y = std::min(ghost_y, cell_y - lowest_mino_rel - 1);
            }
        }
    }
    return ghost_y;
}

bool Active::moveRight() {
    bool can_move = canMoveRight();
    if (can_move) {
        m_x++;
    }
    return can_move;
}

bool Active::canMoveRight() {
    // Get the offset of the rightmost Mino in the grid
    uint8_t rightmost = 0;
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // Spot to the right is filled or lies outside the playfield
                if (p_playfield->isObstructed(m_x + col + 1, m_y + row)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Active::moveLeft() {
    bool can_move = canMoveLeft();
    if (can_move) {
        m_x--;
    }
    return can_move;
}

bool Active::canMoveLeft() {
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // Spot to the left is filled or lies outside playfield
                if (p_playfield->isObstructed(m_x + col - 1, m_y + row)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Active::stepDown() {
    if (canStepDown()) {
        m_y++;
        return true;
    } else {
        return false;
    }
}

bool Active::canStepDown() {
    // Return true if the Tetromino could move down by one cell without
    // colliding with anything

    // Check if already at the bottom or if there would
    // be any collision with a Mino on the p_playfield
    // TODO: Use gridConflict() for this check
    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                // Spot below is filled or lies outside playfield
                if (p_playfield->isObstructed(m_x + col, m_y + row + 1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

int Active::hardDrop() {
    // Hard drop the falling Tetromino, i. e. move it down as far as possible
    // Note that this doesn't lock nor respawn the Tetromino!
    // Returns the number of lines the Tetromino was dropped

    int ghost_y = getGhostY();
    int diff = ghost_y - m_y;
    m_y = ghost_y;
    return diff;
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

bool Active::gridConflict(const TetroGrid_t &grid, int x, int y) {
    // Return true if the given grid, placed at the given location, overlaps
    // with any Mino on the Playfield
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (grid[row][col]) {
                if (p_playfield->isObstructed(x + col, y + row) ||
                    x + col < 0 || x + col >= GRID_SIZE_X || y + col < 0 ||
                    y + col >= GRID_SIZE_Y) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Active::tryWallkicksC(const TetroGrid_t &new_grid, Wallkick_t &success) {
    // Try Wall Kicks for clockwise rotation
    return tryWallkicks(new_grid, 1, success);
}

bool Active::tryWallkicksCC(const TetroGrid_t &new_grid, Wallkick_t &success) {
    // Try Wall Kicks for counterclockwise rotation
    return tryWallkicks(new_grid, -1, success);
}

bool Active::tryWallkicks(const TetroGrid_t &new_grid, int8_t direction,
                          Wallkick_t &success) {
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
    return tryWallkickData(new_grid, wallkick_data, success);
}

bool Active::tryWallkickData(const TetroGrid_t &new_grid,
                             const WallkickData_t *wallkick_data,
                             Wallkick_t &success) {
    // Iterate over all given Wall Kicks to see if one works
    for (uint8_t i = 0; i < wallkick_data->size(); i++) {
        // Check if there would be a conflict using the current Wall Kick
        if (!gridConflict(new_grid, m_x + (*wallkick_data)[i][0],
                          m_y + (*wallkick_data)[i][1])) {
            // Possible Wall Kick found
            success = (*wallkick_data)[i];
            return true;
        }
    }
    // No Wall Kick found
    return false;
}

bool Active::rotateClockw() {
    // If possible, perform a clockwise rotation

    // Generate rotated grid
    TetroGrid_t new_grid = getGridRotatedClockw();
    // Try to perform Wall Kick. Note that no offset (i. e. [0, 0]) is the
    // first Wall Kick that is tried first, therefore it isn't necesarry to
    // exclusively check if the new grid fits without a wall kick.
    Wallkick_t wallkick;
    if (!tryWallkicksC(new_grid, wallkick)) {
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

bool Active::rotateCounterclockw() {
    // If possible, perform a counterclockwise rotation

    TetroGrid_t new_grid = getGridRotatedCounterclockw();
    Wallkick_t wallkick;
    if (!tryWallkicksCC(new_grid, wallkick)) {
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

void Active::draw(SDL_Renderer *renderer) {
    std::array<int, 2> pos;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (m_grid[row][col]) {
                pos = p_playfield->cellToPixelPosition(m_x + col, m_y + row);
                Playfield::drawMino(renderer, pos[0], pos[1],
                                    TETROMINO_COLORS[m_type]);
            }
        }
    }
}

void Active::drawGhost(SDL_Renderer *renderer) {
    int ghost_y = getGhostY();
    //  Don't draw the Ghost if it's at the same position as the actual
    //  Tetromino
    if (ghost_y == m_y) {
        return;
    }
    std::array<int, 2> pos;
    for (int row = 0; row < 4; row++) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (m_grid[row][col]) {
                    pos = p_playfield->cellToPixelPosition(m_x + col,
                                                           ghost_y + row);
                    Playfield::drawGhostMino(renderer, pos[0], pos[1]);
                }
            }
        }
    }
}