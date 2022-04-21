#include <chrono>

#include "SDL.h"

#include "active.h"
#include "constants.h"
#include "bag.h"
#include "hud.h"
#include "timer.h"

/*
 * Handles the main game mechanics.
 */
class Game {
  private:
    // Current m_state of the game
    GameState m_state = GameState::PreInit;

    // When to perform the next fall step
    Timer m_next_fall;
    // When to perform the next soft drop step
    Timer m_next_soft_drop;
    // Whether the Tetromino is currently Soft Dropping, i. e. the down arrow
    // key is m_held
    bool m_soft_dropping = false;
    // When to lock down the active Tetromino
    Timer m_lock_down;
    // Whether the active Tetromino is currently in contact with a Mino on the
    // Playfield; used in combination with m_lock_down Timer
    bool m_surface_contact = false;
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
    bool m_moving_right = false;
    bool m_moving_left = false;
    Timer m_next_mv_right;
    Timer m_next_mv_left;
    void initMoveLeft();
    void stopMoveLeft();
    void moveLeft();
    void initMoveRight();
    void stopMoveRight();
    void moveRight();

    // T-Spins
    // Store the last rotation point (a value in the range [0, 4] determined by
    // what rotation was used by SRS)
    int m_last_rotation_point;
    // Whether the last action was a spin
    bool m_last_spin = false;
    // Check if the last action was a T-Spin or a Mini T-Spin and award points
    // accordingly
    int checkTSpin();
    void getCorners(bool& tl, bool& tr, bool& bl, bool& br);
    bool inTSlot();
    bool inMiniTSlot();

    void hold();
    TetrominoKind_t m_held = -1;
    bool m_can_hold = true;

    SevenBag m_bag;
    HUD m_hud;
    FixedGoalScoring m_scoring = FixedGoalScoring(1);

    void restart();

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

