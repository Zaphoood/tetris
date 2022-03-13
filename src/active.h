#pragma once
#include "array"
#include "stdint.h"

#include "constants.h"
#include "playfield.h"

class Active {
  public:
    Playfield *p_playfield;
    int m_x, m_y; // Top left position of rectangle that contains the Tetromino
    uint8_t m_orientation; // A clockwise rotation increases the orientation by
                           // one, counter-clockwise decreases by one
    TetroGrid_t m_grid;    // grid representation of the current Tetromino

    uint8_t m_type; // Type of current Tetrmino (e.g. O)

  private:
    void loadGrid();
    bool canMoveRight();
    bool canMoveLeft();
    TetroGrid_t getGridRotatedClockw();
    TetroGrid_t getGridRotatedCounterclockw();
    bool gridConflict(const TetroGrid_t &grid, int x, int y);
    bool tryWallkicksC(const TetroGrid_t &new_grid, Wallkick_t &success);
    bool tryWallkicksCC(const TetroGrid_t &new_grid, Wallkick_t &success);
    bool tryWallkicks(const TetroGrid_t &new_grid, int8_t direction,
                      Wallkick_t &success);
    bool tryWallkickData(const TetroGrid_t &new_grid,
                         const WallkickData_t *wallkick_data,
                         Wallkick_t &success);

  public:
    Active();
    Active(uint8_t type, Playfield *p_playfield);

    bool respawn(uint8_t type);
    void lockDown();
    int getGhostY();

    bool moveRight();
    bool moveLeft();
    bool stepDown();
    bool canStepDown();
    int hardDrop();
    bool rotateClockw();
    bool rotateCounterclockw();

    void draw(SDL_Renderer *renderer);
    void drawGhost(SDL_Renderer *renderer);
};