#include <chrono>

#include "SDL.h"

#include "active.h"
#include "playfield.h"

const int FALL_DELAY_MS = 250;

class Game {
  private:
    int randomTetrominoType();
    std::chrono::steady_clock::time_point t_next_step_down;

  public:
    Game();

    Playfield playfield;
    Active active;

    void init();
    void update();
    void handleEvent(const SDL_Event &e);
    void draw(SDL_Renderer *renderer);
};