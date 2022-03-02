#include <chrono>

#include "SDL.h"

#include "active.h"
#include "bag.h"
#include "playfield.h"
#include "tetrovis.h"

class Game {
  private:
    // When to perform the next step down
    std::chrono::steady_clock::time_point t_next_fall;
    // When to perform the next soft drop; A single Soft Drop refers to
    // stepping down one cell at an increased speed as the Down Arrow key is
    // held
    std::chrono::steady_clock::time_point t_next_soft_drop;
    // Whether the Tetromino is currently Soft Dropping, i. e. the Down Arrow
    // key is held
    bool soft_dropping = false;
    // When to lock down the falling Tetromino
    std::chrono::steady_clock::time_point t_lock_down;
    // Whether the falling Tetromino is currently in contact with a Mino on the
    // Playfield; used in combination with t_lock_down
    bool surface_contact = false;
    void startSoftDropping();
    void stopSoftDropping();
    bool performSoftDrop();
    void incSoftDropTimer(std::chrono::steady_clock::time_point last);
    void resetSoftDropTimer();

    bool performFall();
    void incFallTimer(std::chrono::steady_clock::time_point last);
    void resetFallTimer();

    void scheduleLockDown();

    void lockDownActive();

    SevenBag bag;
    std::array<TetroVisual, QUEUE_LEN> queue_visuals;
    void initQueueVisuals();
    void drawQueue(SDL_Renderer *renderer);

  public:
    Game();

    Playfield playfield;
    Active active;

    void init();
    void update();
    void handleEvent(const SDL_Event &e);
    void draw(SDL_Renderer *renderer);
};