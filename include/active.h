#pragma once
#include "array"
#include "stdint.h"

#include "constants.h"
#include "playfield.h"

class Active {
  public:
    // Reference to playfield is necessary in order to set Minos when the
    // active Tetromino is locked down
    Playfield& m_playfield;
    // Top left corner of containing rectangle of the Tetromino
    int m_x, m_y;
    // Orientation of the Tetromino: A clockwise rotation increases the
    // orientation by one, counter-clockwise decreases by one
    uint8_t m_orientation;
    // Grid representation of the current Tetromino
    TetroGrid_t m_grid;
    // Current type of Tetrmino (e.g. O, L, T etc.)
    uint8_t m_type;

  private:
    void loadGrid();
    bool canMoveRight();
    bool canMoveLeft();
    TetroGrid_t getGridRotatedClockw();
    TetroGrid_t getGridRotatedCounterclockw();
    bool gridConflict(const TetroGrid_t &grid, int x, int y);
    bool tryWallkicksC(const TetroGrid_t &new_grid, Wallkick_t &success,
                       int& rotation_point);
    bool tryWallkicksCC(const TetroGrid_t &new_grid, Wallkick_t &success,
                        int& rotation_point);
    bool tryWallkicks(const TetroGrid_t &new_grid, int8_t direction,
                      Wallkick_t &success, int& rotation_point);
    bool tryWallkickData(const TetroGrid_t &new_grid,
                         const WallkickData_t *wallkick_data,
                         Wallkick_t &success, int& rotation_point);

  public:
    Active(uint8_t type, Playfield& p_playfield);

    bool respawn(uint8_t type);
    void lockDown();
    int getGhostY();

    bool moveRight();
    bool moveLeft();
    bool stepDown();
    bool canStepDown();
    int hardDrop();
    bool rotateClockw(int& rotation_point);
    bool rotateCounterclockw(int& rotation_point);

    void draw(SDL_Renderer *renderer);
    void drawGhost(SDL_Renderer *renderer);
};
