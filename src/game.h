#include <chrono>

#include "SDL.h"

#include "active.h"
#include "playfield.h"

class Game {
  private:
    std::chrono::steady_clock::time_point t_next_step_down;
    std::chrono::steady_clock::time_point t_next_soft_drop;
    // Wether a Soft Drop is currently being performed, i. e. the Down Arrow key
    // is held
    bool soft_dropping = false;
    void startSoftDropping();
    void stopSoftDropping();
    void performSoftDrop();
    void incNextSoftDrop(std::chrono::steady_clock::time_point last);
    void incNextStepDown(std::chrono::steady_clock::time_point last);
    int randomTetrominoType();

  public:
    Game();

    Playfield playfield;
    Active active;

    void init();
    void update();
    void handleEvent(const SDL_Event &e);
    void draw(SDL_Renderer *renderer);
};