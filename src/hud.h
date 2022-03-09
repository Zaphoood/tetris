#include <array>

#include "tetrovis.h"

class HUD {
  private:
    // Visual representation of Tetromino queue
    std::array<TetroVisual, QUEUE_LEN> m_queue_visuals;
    // Visual representation of hold Tetromino
    TetroVisual m_hold_visual;

  public:
    HUD();
    HUD(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);

    void setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    void setHold(TetrominoKind_t hold);
    void draw(SDL_Renderer *renderer);
};