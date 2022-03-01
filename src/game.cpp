#include "cstdlib"
#include "iostream"

#include "constants.h"
#include "game.h"

Game::Game() {
    // TODO: See why Playfield() is initialized twice
    playfield = Playfield();
    active = Active(randomTetrominoType());
}

void Game::init() {
    // Schedule the first fall
    std::chrono::steady_clock::time_point t_next_fall;
    resetFallTimer();
    // Initialize PRNG
    srand(time(0));
}

void Game::update() {
    // Check if the falling Tetromino has made surface contact, if so schedule
    // lock down timer
    if (!active.canStepDown(&playfield)) {
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
            lockDownActive();
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
    return active.stepDown(&playfield);
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
    return active.stepDown(&playfield);
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
                active.moveRight(&playfield);
                // Reset Lock Down timer to give the player another 0.5
                // seconds to move the Tetromino before it finally sets
                // This happens for all sidewards movements and rotations
                scheduleLockDown();
                break;
            case SDLK_LEFT:
                active.moveLeft(&playfield);
                scheduleLockDown();
                break;
            case SDLK_DOWN:
                startSoftDropping();
                break;
            case SDLK_UP:
                active.rotateClockw(&playfield);
                scheduleLockDown();
                break;
            case SDLK_c:
                active.rotateCounterclockw(&playfield);
                scheduleLockDown();
                break;
            case SDLK_SPACE:
                active.hardDrop(&playfield);
                lockDownActive();
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

void Game::draw(SDL_Renderer *renderer) {
    playfield.draw(renderer, 0, 0);
    active.drawGhost(renderer, &playfield);
    active.draw(renderer, &playfield);
}

int Game::randomTetrominoType() {
    return rand() % N_TETROMINOS;
}

void Game::lockDownActive() {
    // Lock down and respawn the falling Tetromino and clear empty lines on the
    // Playfield
    active.lockDownAndRespawn(randomTetrominoType(), &playfield);
    playfield.clearEmptyLines();
}