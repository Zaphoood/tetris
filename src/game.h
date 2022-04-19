#include <chrono>

#include "SDL.h"

#include "active.h"
#include "bag.h"
#include "hud.h"
#include "playfield.h"
#include "scoring.h"
#include "tetrovis.h"
#include "timer.h"

enum class GameState { PreInit, Running, GameOver };

/*
 * Handles the main game mechanics.
 */
class Game {
  private:
    // Current state of the game
    GameState state = GameState::PreInit;

    // When to perform the next fall step
    Timer next_fall;
    // When to perform the next soft drop step
    Timer next_soft_drop;
    // Whether the Tetromino is currently Soft Dropping, i. e. the down arrow
    // key is held
    bool soft_dropping = false;
    // When to lock down the active Tetromino
    Timer lock_down;
    // Whether the active Tetromino is currently in contact with a Mino on the
    // Playfield; used in combination with lock_down Timer
    bool surface_contact = false;
    void startSoftDropping();
    void stopSoftDropping();
    bool performSoftDrop();
    void incSoftDropTimer();
    void resetSoftDropTimer();

    bool performFall();
    void incFallTimer();
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

    // T-Spins
    // Store the last rotation point (a value in the range [0, 4] determined by
    // what rotation was used by SRS)
    int rotation_point;
    // Whether the last action was a spin
    bool last_spin = false;
    // Check if the last action was a T-Spin or a Mini T-Spin and award points
    // accordingly
    int checkTSpin();
    void getCorners(bool& tl, bool& tr, bool& bl, bool& br);
    bool inTSlot();
    bool inMiniTSlot();

    void hold();
    TetrominoKind_t held = -1;
    bool can_hold = true;

    SevenBag bag;
    HUD hud;
    FixedGoalScoring scoring = FixedGoalScoring(1);

    bool paused = false;

  public:
    Game(const std::string& assets_path);

    Playfield playfield;
    Active active;

    void init();
    void update();
    GameState getState();
    void handleEvent(const SDL_Event &e);
    void draw(SDL_Renderer *renderer);
};

