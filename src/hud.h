#include <array>

#include "tetrovis.h"

class HUD {
  private:
    std::array<TetroVisual, QUEUE_LEN> m_queue_visuals;

  public:
    HUD();
    HUD(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);

    void setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    void draw(SDL_Renderer *renderer);
};