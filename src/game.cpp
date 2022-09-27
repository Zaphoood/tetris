#include <iostream>
#include <thread>

#include "SDL_keycode.h"
#include "game.h"

using cl = std::chrono::steady_clock;

Game::Game(const std::string& assets_path)
    : playfield(PLAYFIELD_DRAW_X, PLAYFIELD_DRAW_Y),
      active(m_bag.popQueue(), playfield), m_hud(assets_path, m_scoring), m_scoring() {
    // Don't set m_hud's queue in initializer list since the order of
    // initialization is not guaranteed
    respawnActive();
}

void Game::init() {
    restart();
}

void Game::restart() {
    // Reset some member variables
    m_surface_contact = false;
    m_moving_right = false;
    m_moving_left = false;
    m_last_spin = false;
    TetrominoKind_t m_held = -1;
    bool m_can_hold = true;

    // Schedule the first fall
    resetFallTimer();
    m_state = GameState::Running;
    playfield.reset();
    m_bag.reset();
    active.respawn(m_bag.popQueue());
    m_hud.reset();
    m_hud.setQueue(m_bag.getQueue());
    m_scoring = FixedGoalScoring(1);
}

/**
 * Main update function, handles game logic
 */
void Game::update() {
    if (m_state != GameState::Running) {
        return;
    }

    cl::time_point now =
        cl::now();
    if (m_moving_right) {
        if (m_next_mv_right < now) {
            moveRight();
        }
    }
    if (m_moving_left) {
        if (m_next_mv_left < now) {
            moveLeft();
        }
    }

    // Check if the falling Tetromino has made surface contact; if so, schedule lock down timer
    if (!active.canStepDown()) {
        if (!m_surface_contact) {
            m_surface_contact = true;
            scheduleLockDown();
        }
    } else {
        if (m_surface_contact) {
            // No more surface contact; resume falling / soft dropping
            m_surface_contact = false;
            // Reset all timers
            resetFallTimer();
            resetSoftDropTimer();
        }
    }

    if (m_surface_contact) {
        if (m_lock_down < now) {
            // Making surface contact and lock down timer has run out
            // Lock down Tetromino and spawn a new one
            lockDownAndRespawnActive();
            // If possible, step down immediatly after respawning (this is
            // according to the Tetris Guideline)
            performFall();
        }
    } else {
        if (m_soft_dropping && m_next_soft_drop < now) {
            performSoftDrop();
        } else if (m_next_fall < now) {
            performFall();
        }
    }

    // Limit framerate; note that the variable `now` holds the time since epoch at
    // the start of this frame
    std::this_thread::sleep_for(std::chrono::milliseconds(
          MIN_FRAMETIME_MS - (std::chrono::duration_cast<std::chrono::milliseconds>(cl::now() - now)).count()));
}

GameState Game::getState() {
    return m_state;
}

/**
 * Start soft dropping and immediately perform first soft drop
 */
void Game::startSoftDropping() {
    m_soft_dropping = true;
    resetSoftDropTimer();
    performSoftDrop();
}

/**
 * Stop soft dropping and resume normal falling
 */
void Game::stopSoftDropping() {
    m_soft_dropping = false;
    resetFallTimer();
}

bool Game::performSoftDrop() {
    incSoftDropTimer();
    m_scoring.onSoftDrop();
    return active.stepDown();
}

/**
 * Reset the soft drop timer
 */
void Game::resetSoftDropTimer() {
    m_next_soft_drop = cl::now() +
        std::chrono::milliseconds((int) (m_scoring.getFallSpeedMs() * SOFT_DROP_DELAY_MULT));
}

/**
 * Schedule the next soft drop
 */
void Game::incSoftDropTimer() {
    m_next_soft_drop = m_next_soft_drop +
        std::chrono::milliseconds((int) (m_scoring.getFallSpeedMs() * SOFT_DROP_DELAY_MULT));
}

/**
 * Move the Tetromino down by one cell and set the timer for the next fall move
 */
bool Game::performFall() {
    incFallTimer();
    return active.stepDown();
}

/**
 * Reset the fall timer.
 */
void Game::resetFallTimer() {
    m_next_fall = cl::now() + std::chrono::milliseconds(m_scoring.getFallSpeedMs());
}

/**
 * Schedule the next fall
 */
void Game::incFallTimer() {
    m_next_fall = m_next_fall + std::chrono::milliseconds(m_scoring.getFallSpeedMs());
}

void Game::scheduleLockDown() {
    m_lock_down = cl::now() + std::chrono::milliseconds(LOCK_DOWN_DELAY_MS);
}

/**
 * Handle any event. Should be called by the main loop with all events that occur.
 *
 * @param an event
 */
void Game::handleEvent(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        // Ignore repeated keys, we implement our own repeated inputs
        if (!e.key.repeat) {
            switch (e.key.keysym.sym) {
            case SDLK_RIGHT:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    initMoveRight();
                    m_last_spin = false;
                }
                break;
            case SDLK_LEFT:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    initMoveLeft();
                    m_last_spin = false;
                }
                break;
            case SDLK_DOWN:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    startSoftDropping();
                    m_last_spin = false;
                }
                break;
            case SDLK_UP:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    active.rotateClockw(m_last_rotation_point);
                    m_last_spin = true;
                    scheduleLockDown();
                }
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    active.rotateCounterclockw(m_last_rotation_point);
                    m_last_spin = true;
                    scheduleLockDown();
                }
                break;
            case SDLK_SPACE:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    m_scoring.onHardDrop(active.hardDrop());
                    lockDownAndRespawnActive();
                    m_last_spin = false;
                }
                break;
            case SDLK_c:
                if (m_state == GameState::Paused) break;
                if (m_state == GameState::Running) {
                    hold();
                    m_last_spin = false;
                }
                break;
            case SDLK_ESCAPE:
                if (m_state == GameState::Paused) {
                    m_next_fall.resume();
                    m_next_soft_drop.resume();
                }
                else {
                    m_next_fall.pause();
                    m_next_soft_drop.pause();
                }
                if (m_state == GameState::Paused) {
                    m_state = GameState::Running;
                } else if (m_state == GameState::Running) {
                    m_state = GameState::Paused;
                }
                break;
            case SDLK_RETURN:
                if (m_state == GameState::GameOver) {
                    restart();
                }
                break;
            }
        }
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.sym) {
        case SDLK_RIGHT:
            stopMoveRight();
            break;
        case SDLK_LEFT:
            stopMoveLeft();
            break;
        case SDLK_DOWN:
            m_soft_dropping = false;
            break;
        }
    }
}

/**
 * Start moving the active Tetromino to the right repeatedly
 */
void Game::initMoveRight() {
    if (!m_moving_right) {
        m_moving_right = true;
        m_moving_left = false;
        moveRight();
        // Override the timer for the next right move set by moveRight
        // Instead, set initial delay
        m_next_mv_right = cl::now() +
                          std::chrono::milliseconds(KEY_INIT_DELAY_MS);
    }
}

/**
 * Stop moving the active Tetromino to the right
 */
void Game::stopMoveRight() {
    m_moving_right = false;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT] && m_state == GameState::Running) {
        initMoveLeft();
    }
}

/**
 * Move the active Tetromino to the right by one cell
 */
void Game::moveRight() {
    if (active.moveRight()) {
        // If the move was successfull, reset Lock Down timer to give the
        // player another 0.5 seconds to move the Tetromino before it finally
        // sets. (This happens for all sidewards movements and rotations) Only
        // reset lockdown timer if the move was successfull
        scheduleLockDown();
    }
    m_next_mv_right += std::chrono::milliseconds(KEY_REPEAT_DELAY_MS);
}

/**
 * Start moving the active Tetromino to the left repeatedly
 */
void Game::initMoveLeft() {
    if (!m_moving_left) {
        m_moving_left = true;
        m_moving_right = false;
        moveLeft();
        // Override the timer for the next left move set by moveLeft
        // Instead, set initial delay
        m_next_mv_left = cl::now() +
                         std::chrono::milliseconds(KEY_INIT_DELAY_MS);
    }
}

/**
 * Stop moving the active Tetromino to the left
 */
void Game::stopMoveLeft() {
    m_moving_left = false;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_RIGHT] && m_state == GameState::Running) {
        initMoveRight();
    }
}

/**
 * Move the active Tetromino to the left by one cell
 */
void Game::moveLeft() {
    if (active.moveLeft()) {
        // Only reset lockdown timer if the move was successfull
        scheduleLockDown();
    }
    m_next_mv_left += std::chrono::milliseconds(KEY_REPEAT_DELAY_MS);
}

/**
 * Put the current Tetromino in the "Hold" area and replace it with the Tetromino
 * that is currently m_held, spawning it at the top of the Playfield
 */
void Game::hold() {
    if (m_can_hold) {
        // Disable hold until next piece is set
        m_can_hold = false;
        // 255 corrsponds to no value set
        if (m_held == 255) {
            // Set kind of m_held Tetromino to be the old active Tetrominos
            // kind
            m_held = active.m_type;
            // Respawn Tetromino as new random kind
            respawnActive();
        } else {
            // Store intermediate value
            TetrominoKind_t active_kind = active.m_type;
            // Set kind of respawned Tetromino to `m_held`
            respawnActiveWithKind(m_held);
            // Set kind of m_held Tetromino to be the old active Tetrominos
            // kind
            m_held = active_kind;
        }
        m_hud.setHold(m_held);
    }
}

void Game::draw(SDL_Renderer *renderer) {
    playfield.draw(renderer);
    active.drawGhost(renderer);
    active.draw(renderer);
    m_hud.draw(renderer, m_state);
}


/**
 * Check whether the last lock down constitutes a T-Spin or a Mini T-Spin
 *
 * @return 2 for T-Spin, 1 for Mini T-Spin, otherwise 0
 */
int Game::checkTSpin() {
    // Assert that the current Tetromino is a type T one and that the last input
    // was a rotation
    if (active.m_type != 5 || !m_last_spin) {
        return 0;
    }

    // Check whether the last rotation was a T-Spin. Must be called right before
    // lock down occurs
    if (inTSlot() || m_last_rotation_point == 5) {
        // T-Spin
        return 2;
    } else if (inMiniTSlot()) {
        // Mini T-Spin
        return 1;
    }
    return 0;
}

/**
 * Check if the corners surrounding a T-Tetromino are obstructed.
 */
void Game::getCorners(bool& tl, bool& tr, bool& bl, bool& br) {
    // TODO: Maybe use bitmask instead of bools
    // clang-format off
    tl = playfield.isObstructed(active.m_x,     active.m_y);
    tr = playfield.isObstructed(active.m_x + 2, active.m_y);
    bl = playfield.isObstructed(active.m_x,     active.m_y + 2);
    br = playfield.isObstructed(active.m_x + 2, active.m_y + 2);
    // clang-format on
}

/**
 * Check if the active Tetromino is currently in a T-Slot
 *
 * @return the above
 */
bool Game::inTSlot() {
    bool tl, tr, bl, br;
    getCorners(tl, tr, bl, br);

    switch (active.m_orientation) {
    case 0: // Facing north
        return tl && tr && (bl || br);
        break;
    case 1: // Facing east
        return tr && br && (tl || bl);
        break;
    case 2: // Facing south
        return br && bl && (tr || tl);
        break;
    case 3: // Facing west
        return bl && tl && (br || tr);
        break;
    default:
        std::cout << "Unreachable: Rotation not in [0..3]\n";
        exit(1);
    }
}

/**
 * Check if the active Tetromino is currently in a Mini T-Slot
 *
 * @return the above
 */
bool Game::inMiniTSlot() {
    bool tl, tr, bl, br;
    getCorners(tl, tr, bl, br);

    switch (active.m_orientation) {
    case 0: // Facing north
        return (tl || tr) && bl && br;
        break;
    case 1: // Facing east
        return (tr || br) && tl && bl;
        break;
    case 2: // Facing south
        return (br || bl) && tr && tl;
        break;
    case 3: // Facing west
        return (bl || tl) && br && tr;
        break;
    default:
        std::cout << "Unreachable: Rotation not in [0..3]\n";
        exit(1);
    }
}

/*
 * Lock down the falling Tetromino, then respawn. If respawn was successful,
 * clear empty lines on the Playfield.
 */
void Game::lockDownAndRespawnActive() {

    int t_spin = checkTSpin();
    active.lockDown();
    if (respawnActive()) {
        int cleared = playfield.clearEmptyLines();
        switch (t_spin) {
        case 0:
            m_scoring.onLinesCleared(cleared);
            break;
        case 1:
            std::cout << "Mini T-Spin, clearing " << cleared << " lines!\n";
            m_scoring.onMiniTSpin(cleared);
            break;
        case 2:
            std::cout << "T-Spin, clearing " << cleared << " lines!\n";
            m_scoring.onTSpin(cleared);
            break;
        }
        resetFallTimer();
    }
    // Re-enable hold
    m_can_hold = true;
}

/*
 * Respawn the active Tetromino, setting its kind to the next value from
 * the queue
 */
bool Game::respawnActive() {
    return Game::respawnActiveWithKind(m_bag.popQueue());
}

/*
 * Respawn the active Tetromino, settings its kind to the given value
 */
bool Game::respawnActiveWithKind(TetrominoKind_t kind) {
    if (!active.respawn(kind)) {
        // Block Out
        m_state = GameState::GameOver;
        return false;
    }
    // Move down own cell immediatly after respawning; this is according to
    // the Tetromino Guideline
    active.stepDown();
    m_hud.setQueue(m_bag.getQueue());
    return true;
}

