#include <chrono>

#include "SDL.h"

#include "active.h"
#include "bag.h"
#include "hud.h"
#include "playfield.h"
#include "scoring.h"
#include "tetrovis.h"

enum class GameState { PreInit, Running, GameOver };

class Game {
  private:
    // Current state of the game
    GameState state = GameState::PreInit;

    // ### Controls ###
    // When to perform the next fall step
    std::chrono::steady_clock::time_point t_next_fall;
    // When to perform the next soft drop step
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
    void lockDownAndRespawnActive();
    bool respawnActive();
    bool respawnActiveWithKind(TetrominoKind_t kind);

    // Horizontal movement
    const Uint8 *keystate;
    bool moving_right = false;
    bool moving_left = false;
    std::chrono::steady_clock::time_point t_next_mv_right;
    std::chrono::steady_clock::time_point t_next_mv_left;
    void initMoveLeft();
    void stopMoveLeft();
    void moveLeft();
    void initMoveRight();
    void stopMoveRight();
    void moveRight();

    void hold();
    TetrominoKind_t held = -1;
    bool can_hold = true;

    SevenBag bag;
    HUD hud;
    FixedGoalScoring scoring = FixedGoalScoring(1);

  public:
    Game();

    Playfield playfield;
    Active active;

    void init();
    void update();
    GameState getState();
    void handleEvent(const SDL_Event &e);
    void draw(SDL_Renderer *renderer);
};