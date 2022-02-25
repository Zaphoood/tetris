#pragma once
#include "array"
#include "stdint.h"

inline const int STARTING_POSITION_X = 3;
inline const int STARTING_POSITION_Y = 20;

class Active {
  public:
    int m_x, m_y; // Top left position of rectangle that contains the Tetromino
    std::array<std::array<uint8_t, 4>, 4>
        m_grid; // grid representation of the current Tetromino

    uint8_t m_type; // Type of current Tetrmino (e.g. O)

  private:
    void loadTetromino();

  public:
    Active(uint8_t type);

    void respawn(uint8_t type);
    void move_right();
    void move_left();
    void step_down();
};