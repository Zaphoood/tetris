#include "cstdlib"
#include "iostream"

#include "constants.h"
#include "game.h"

Game::Game()
    : playfield(PLAYFIELD_DRAW_X, PLAYFIELD_DRAW_Y),
      active(bag.popQueue(), &playfield) {
    // Don't set hud's queue in initializer list since the order of
    // initialization is not guaranteed
    hud.setQueue(bag.getQueue());
}

void Game::init() {
    // Schedule the first fall
    std::chrono::steady_clock::time_point t_next_fall;
    resetFallTimer();
    state = GameState::Running;
}

void Game::update() {
    // Check if the falling Tetromino has made surface contact, if so schedule
    // lock down timer
    if (state != GameState::Running) {
        return;
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
        if (std::chrono::steady_clock::now() > t_lock_down) {
            // Making surface contact and lock down timer has run out
            // Lock down Tetromino and spawn a new one
            lockDownAndRespawnActive();
            // If possible, step down immediatly after respawning (this is
            // according to the Tetris Guideline)
            performFall();
        }
    } else {
        if (soft_dropping &&
            std::chrono::steady_clock::now() > t_next_soft_drop) {
            performSoftDrop();
        } else if (std::chrono::steady_clock::now() > t_next_fall) {
            performFall();
        }
    }
}

GameState Game::getState() {
    return state;
}

void Game::startSoftDropping() {
    // Start soft dropping and immediatly perform first soft drop
    soft_dropping = true;
    resetSoftDropTimer();
    performSoftDrop();
}

void Game::stopSoftDropping() {
    // Stop soft dropping and resume normal falling
    soft_dropping = false;
    resetFallTimer();
}

bool Game::performSoftDrop() {
    incSoftDropTimer(t_next_soft_drop);
    return active.stepDown();
}

void Game::resetSoftDropTimer() {
    // Reset the soft drop timer. Should be called when soft dropping is started
    incSoftDropTimer(std::chrono::steady_clock::now());
}

void Game::incSoftDropTimer(std::chrono::steady_clock::time_point last) {
    // Schedule the next soft drop, as an offset from last
    t_next_soft_drop = last + std::chrono::milliseconds(
                                  (int)(FALL_DELAY_MS * SOFT_DROP_DELAY_MULT));
}

bool Game::performFall() {
    // Move the Tetromino down by one cell and set the timer for the next fall
    // move
    incFallTimer(t_next_fall);
    return active.stepDown();
}

void Game::resetFallTimer() {
    // Reset the fall timer. Should be called when Tetromino starts to fall
    incFallTimer(std::chrono::steady_clock::now());
}

void Game::incFallTimer(std::chrono::steady_clock::time_point last) {
    // Schedule the next fall, as an offset from last
    t_next_fall = last + std::chrono::milliseconds(FALL_DELAY_MS);
}

void Game::scheduleLockDown() {
    t_lock_down = std::chrono::steady_clock::now() +
                  std::chrono::milliseconds(LOCK_DOWN_DELAY_MS);
}

void Game::handleEvent(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        // Ignore repeated keys, we implement our own repeated inputs
        if (!e.key.repeat) {
            switch (e.key.keysym.sym) {
            case SDLK_RIGHT:
                if (state == GameState::Running) {
                    active.moveRight();
                    // Reset Lock Down timer to give the player another 0.5
                    // seconds to move the Tetromino before it finally sets
                    // This happens for all sidewards movements and rotations
                    scheduleLockDown();
                }
                break;
            case SDLK_LEFT:
                if (state == GameState::Running) {
                    active.moveLeft();
                    scheduleLockDown();
                }
                break;
            case SDLK_DOWN:
                if (state == GameState::Running) {
                    startSoftDropping();
                }
                break;
            case SDLK_UP:
                if (state == GameState::Running) {
                    active.rotateClockw();
                    scheduleLockDown();
                }
                break;
            case SDLK_LCTRL:
            case SDLK_RCTRL:
                if (state == GameState::Running) {
                    active.rotateCounterclockw();
                    scheduleLockDown();
                }
                break;
            case SDLK_SPACE:
                if (state == GameState::Running) {
                    active.hardDrop();
                    lockDownAndRespawnActive();
                }
                break;
            case SDLK_c:
                if (state == GameState::Running) {
                    hold();
                }
                break;
            }
        }
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.sym) {
        case SDLK_DOWN:
            soft_dropping = false;
            break;
        }
    }
}

void Game::hold() {
    if (can_hold) {
        // Disable hold until next piece is set
        can_hold = false;
        if (held == (TetrominoKind_t)-1) {
            // Set kind of held Tetromino to be the old active Tetrominos kind
            held = active.m_type;
            // Respawn Tetromino as new random kind
            respawnActive();
        } else {
            // Store intermediate value
            TetrominoKind_t active_kind = active.m_type;
            // Set kind of respawned Tetromino to `held`
            respawnActiveWithKind(held);
            // Set kind of held Tetromino to be the old active Tetrominos kind
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

void Game::lockDownAndRespawnActive() {
    // Lock down the falling Tetromino, then respawn
    // If respawn was successfull, clear empty lines on the Playfield
    active.lockDown();
    if (respawnActive()) {
        playfield.clearEmptyLines();
    }
    // Re-enable hold
    can_hold = true;
}

bool Game::respawnActive() {
    // Respawn the active Tetromino, setting its kind to the next value from the
    // queue
    return Game::respawnActiveWithKind(bag.popQueue());
}

bool Game::respawnActiveWithKind(TetrominoKind_t kind) {
    // Respawn the active Tetromino, settings its kind to the given value
    if (!active.respawn(kind)) {
        // Block Out
        state = GameState::GameOver;
        return false;
    }
    hud.setQueue(bag.getQueue());
    return true;
}