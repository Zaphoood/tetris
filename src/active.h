#pragma once
#include "array"
#include "playfield.h"
#include "stdint.h"

inline const int STARTING_POSITION_X = 3;  // 3;
inline const int STARTING_POSITION_Y = 28; // 20;

class Active {
  public:
    int m_x, m_y; // Top left position of rectangle that contains the Tetromino
    std::array<std::array<uint8_t, 4>, 4>
        m_grid; // grid representation of the current Tetromino

    uint8_t m_type; // Type of current Tetrmino (e.g. O)

  private:
    void loadGrid();
    bool canMoveRight(Playfield *playfield);
    bool canMoveLeft(Playfield *playfield);

  public:
    Active(uint8_t type);

    void respawn(uint8_t type);
    void setAndRespawn(uint8_t type, Playfield *playfield);
    void draw(SDL_Renderer *renderer, Playfield *playfield);
    bool canStepDown(Playfield *playfield);
    bool moveRight(Playfield *playfield);
    bool moveLeft(Playfield *playfield);
    bool stepDown(Playfield *playfield);
};