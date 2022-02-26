#pragma once
#include "array"
#include "stdint.h"

#include "playfield.h"

// Four-by-four Matrix containing the grid representation of a Tetromino
using TetroGrid_t = std::array<std::array<uint8_t, 4>, 4>;
// A single Wall Kick
using Wallkick_t = std::array<int8_t, 2>;
// Set of all 5 Wall Kicks to try for a combination of Tetromino type and
// direction of rotation
using WallkickData_t = std::array<Wallkick_t, 5>;

class Active {
  public:
    int m_x, m_y; // Top left position of rectangle that contains the Tetromino
    uint8_t m_orientation; // A clockwise rotation increases the orientation by
                           // one, counter-clockwise decreases by one
    TetroGrid_t m_grid;    // grid representation of the current Tetromino

    uint8_t m_type; // Type of current Tetrmino (e.g. O)

  private:
    void loadGrid();
    bool canMoveRight(Playfield *playfield);
    bool canMoveLeft(Playfield *playfield);
    TetroGrid_t getGridRotatedClockw();
    TetroGrid_t getGridRotatedCounterclockw();
    bool doesGridConflict(const TetroGrid_t &grid, Playfield *playfield);
    bool doesOffsetGridConflict(const TetroGrid_t &grid, int dx, int dy,
                                Playfield *playfield);
    bool tryWallkicksC(const TetroGrid_t &new_grid, Wallkick_t &success,
                       Playfield *playfield);
    bool tryWallkicksCC(const TetroGrid_t &new_grid, Wallkick_t &success,
                        Playfield *playfield);
    bool tryWallkicks(const TetroGrid_t &new_grid, int8_t direction,
                      Wallkick_t &success, Playfield *playfield);
    bool tryWallkickData(const TetroGrid_t &new_grid,
                         const WallkickData_t *wallkick_data,
                         Wallkick_t &success, Playfield *playfield);

  public:
    Active();
    Active(uint8_t type);

    void respawn(uint8_t type);
    void setAndRespawn(uint8_t type, Playfield *playfield);
    void draw(SDL_Renderer *renderer, Playfield *playfield);
    bool canStepDown(Playfield *playfield);
    bool moveRight(Playfield *playfield);
    bool moveLeft(Playfield *playfield);
    bool stepDown(Playfield *playfield);
    bool rotateClockw(Playfield *playfield);
    bool rotateCounterclockw(Playfield *playfield);
};