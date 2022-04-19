#include "cstdlib"
#include "iostream"

#include "constants.h"
#include "game.h"

using cl = std::chrono::steady_clock;

Game::Game(const std::string& assets_path)
    : playfield(PLAYFIELD_DRAW_X, PLAYFIELD_DRAW_Y),
      active(bag.popQueue(), playfield), hud(assets_path, scoring) {
    // Don't set hud's queue in initializer list since the order of
    // initialization is not guaranteed
    respawnActive();
}

void Game::init() {
    // Schedule the first fall
    resetFallTimer();
    state = GameState::Running;
}

void Game::update() {
    // Check if the falling Tetromino has made surface contact, if so schedule
    // lock down timer
    if (state != GameState::Running || paused) {
        return;
    }

    cl::time_point now =
        cl::now();
    if (moving_right) {
        if (now > t_next_mv_right) {
            moveRight();
        }
    }
    if (moving_left) {
        if (now > t_next_mv_left) {
            moveLeft();
        }
    }

    if (!active.canStepDown()) {
        if (!surface_contact) {
            surface_contact = true;
            scheduleLockDown();
        }
    } else {
        if (surface_contact) {
            // No more surface contact; resume falling / soft dropping
            surface_contact = false;
            // Reset all timers
            resetFallTimer();
            resetSoftDropTimer();
        }
    }

    if (surface_contact) {
        if (lock_down < now) {
            // Making surface contact and lock down timer has run out
            // Lock down Tetromino and spawn a new one
            lockDownAndRespawnActive();
            // If possible, step down immediatly after respawning (this is
            // according to the Tetris Guideline)
            performFall();
        }
    } else {
        if (soft_dropping && next_soft_drop < now) {
            performSoftDrop();
        } else if (next_fall < now) {
            performFall();
        }
    }
}

GameState Game::getState() {
    return state;
}

/**
 * Start soft dropping and immediately perform first soft drop
 */
void Game::startSoftDropping() {
    soft_dropping = true;
    resetSoftDropTimer();
    performSoftDrop();
}

/**
 * Stop soft dropping and resume normal falling
 */
void Game::stopSoftDropping() {
    soft_dropping = false;
    resetFallTimer();
}

bool Game::performSoftDrop() {
    incSoftDropTimer();
    scoring.onSoftDrop();
    return active.stepDown();
}

/**
 * Reset the soft drop timer
 */
void Game::resetSoftDropTimer() {
    next_soft_drop = cl::now() +
        std::chrono::milliseconds((int) (scoring.getFallSpeedMs() * SOFT_DROP_DELAY_MULT));
}

/**
 * Schedule the next soft drop
 */
void Game::incSoftDropTimer() {
    next_soft_drop = next_soft_drop +
        std::chrono::milliseconds((int) (scoring.getFallSpeedMs() * SOFT_DROP_DELAY_MULT));
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
    next_fall = cl::now() + std::chrono::milliseconds(scoring.getFallSpeedMs());
}

/**
 * Schedule the next fall
 */
void Game::incFallTimer() {
    next_fall = next_fall + std::chrono::milliseconds(scoring.getFallSpeedMs());
}

void Game::scheduleLockDown() {
    lock_down = cl::now() + std::chrono::milliseconds(LOCK_DOWN_DELAY_MS);
}

void Game::handleEvent(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        // Ignore repeated keys, we implement our own repeated inputs
        if (!e.key.repeat) {
            switch (e.key.keysym.sym) {
            case SDLK_RIGHT:
                if (state == GameState::Running) {
                    initMoveRight();
                    last_spin = false;
                }
                break;
            case SDLK_LEFT:
                if (state == GameState::Running) {
                    initMoveLeft();
                    last_spin = false;
                }
                break;
            case SDLK_DOWN:
                if (state == GameState::Running) {
                    startSoftDropping();
                    last_spin = false;
                }
                break;
            case SDLK_UP:
                if (state == GameState::Running) {
                    active.rotateClockw(rotation_point);
                    last_spin = true;
                    scheduleLockDown();
                }
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if (state == GameState::Running) {
                    active.rotateCounterclockw(rotation_point);
                    last_spin = true;
                    scheduleLockDown();
                }
                break;
            case SDLK_SPACE:
                if (state == GameState::Running) {
                    scoring.onHardDrop(active.hardDrop());
                    lockDownAndRespawnActive();
                    last_spin = false;
                }
                break;
            case SDLK_c:
                if (state == GameState::Running) {
                    hold();
                    last_spin = false;
                }
                break;
            case SDLK_p:
                // TODO: Pause timers
                paused = !paused;
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
            soft_dropping = false;
            break;
        }
    }
}

void Game::initMoveRight() {
    if (!moving_right) {
        moving_right = true;
        moving_left = false;
        moveRight();
        // Override the timer for the next right move set by moveRight
        // Instead, set initial delay
        t_next_mv_right = cl::now() +
                          std::chrono::milliseconds(KEY_INIT_DELAY_MS);
    }
}

void Game::stopMoveRight() {
    moving_right = false;
    keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_LEFT]) {
        initMoveLeft();
    }
}

void Game::moveRight() {
    if (active.moveRight()) {
        // If the move was successfull, reset Lock Down timer to give the
        // player another 0.5 seconds to move the Tetromino before it finally
        // sets. (This happens for all sidewards movements and rotations) Only
        // reset lockdown timer if the move was successfull
        scheduleLockDown();
    }
    t_next_mv_right += std::chrono::milliseconds(KEY_REPEAT_DELAY_MS);
}

void Game::initMoveLeft() {
    if (!moving_left) {
        moving_left = true;
        moving_right = false;
        moveLeft();
        // Override the timer for the next left move set by moveLeft
        // Instead, set initial delay
        t_next_mv_left = cl::now() +
                         std::chrono::milliseconds(KEY_INIT_DELAY_MS);
    }
}

void Game::stopMoveLeft() {
    moving_left = false;
    keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_RIGHT]) {
        initMoveRight();
    }
}

void Game::moveLeft() {
    if (active.moveLeft()) {
        // Only reset lockdown timer if the move was successfull
        scheduleLockDown();
    }
    t_next_mv_left += std::chrono::milliseconds(KEY_REPEAT_DELAY_MS);
}

void Game::hold() {
    if (can_hold) {
        // Disable hold until next piece is set
        can_hold = false;
        // 255 corrsponds to no value set
        if (held == 255) {
            // Set kind of held Tetromino to be the old active Tetrominos
            // kind
            held = active.m_type;
            // Respawn Tetromino as new random kind
            respawnActive();
        } else {
            // Store intermediate value
            TetrominoKind_t active_kind = active.m_type;
            // Set kind of respawned Tetromino to `held`
            respawnActiveWithKind(held);
            // Set kind of held Tetromino to be the old active Tetrominos
            // kind
            held = active_kind;
        }
        hud.setHold(held);
    }
}

void Game::draw(SDL_Renderer *renderer) {
    playfield.draw(renderer);
    active.drawGhost(renderer);
    active.draw(renderer);
    hud.draw(renderer);
}

int Game::checkTSpin() {
    // Assert that the current Tetromino is a type T one and that the last input
    // was a rotation
    if (active.m_type != 5 || !last_spin) {
        return 0;
    }

    // Check whether the last rotation was a T-Spin. Must be called right before
    // lock down occurs
    if (inTSlot() || rotation_point == 5) {
        // T-Spin
        return 2;
    } else if (inMiniTSlot()) {
        // Mini T-Spin
        return 1;
    }
    return 0;
}

void Game::getCorners(bool& tl, bool& tr, bool& bl, bool& br) {
    // TODO: Maybe use bitmasks instead of bools
    // clang-format off
    tl = playfield.isObstructed(active.m_x,     active.m_y);
    tr = playfield.isObstructed(active.m_x + 2, active.m_y);
    bl = playfield.isObstructed(active.m_x,     active.m_y + 2);
    br = playfield.isObstructed(active.m_x + 2, active.m_y + 2);
    // clang-format on
}

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

void Game::lockDownAndRespawnActive() {
    // Lock down the falling Tetromino, then respawn
    // If respawn was successfull, clear empty lines on the Playfield

    int t_spin = checkTSpin();
    active.lockDown();
    if (respawnActive()) {
        int cleared = playfield.clearEmptyLines();
        switch (t_spin) {
        case 0:
            scoring.onLinesCleared(cleared);
            break;
        case 1:
            std::cout << "Mini T-Spin, clearing " << cleared << " lines!\n";
            scoring.onMiniTSpin(cleared);
            break;
        case 2:
            std::cout << "T-Spin, clearing " << cleared << " lines!\n";
            scoring.onTSpin(cleared);
            break;
        }
        resetFallTimer();
    }
    // Re-enable hold
    can_hold = true;
}

bool Game::respawnActive() {
    // Respawn the active Tetromino, setting its kind to the next value from
    // the queue
    return Game::respawnActiveWithKind(bag.popQueue());
}

bool Game::respawnActiveWithKind(TetrominoKind_t kind) {
    // Respawn the active Tetromino, settings its kind to the given value
    if (!active.respawn(kind)) {
        // Block Out
        state = GameState::GameOver;
        return false;
    }
    // Move down own cell immediatly after respawning; this is according to
    // the Tetromino Guideline
    active.stepDown();
    hud.setQueue(bag.getQueue());
    return true;
}
