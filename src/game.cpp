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
    // Schedule the first step down
    std::chrono::steady_clock::time_point t_next_step_down;
    incNextStepDown(std::chrono::steady_clock::now());
}

void Game::update() {
    if (soft_dropping && std::chrono::steady_clock::now() > t_next_soft_drop) {
        performSoftDrop();
    } else {
        if (std::chrono::steady_clock::now() > t_next_step_down) {
            /*
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                             t_next_step_down.time_since_epoch())
                             .count()
                      << "\n";
                      */
            incNextStepDown(t_next_step_down);
            if (!active.stepDown(&playfield)) {
                active.lockDownAndRespawn(randomTetrominoType(), &playfield);
                // If possible, step down immediatly after spawning (this is
                // according to the Tetris Guideline)
                active.stepDown(&playfield);
            }
        }
    }
}

void Game::startSoftDropping() {
    // Start soft dropping and immediatly perform first soft drop
    soft_dropping = true;
    incNextSoftDrop(std::chrono::steady_clock::now());
    performSoftDrop();
}

void Game::stopSoftDropping() {
    // Stop soft dropping and resume normal falling (i. e. stepDown()-ing)
    soft_dropping = false;
    incNextStepDown(std::chrono::steady_clock::now());
}

void Game::performSoftDrop() {
    incNextSoftDrop(t_next_soft_drop);
    active.stepDown(&playfield);
}

void Game::incNextSoftDrop(std::chrono::steady_clock::time_point last) {
    // Schedule the next soft drop, as an offset from last
    t_next_soft_drop = last + std::chrono::milliseconds(
                                  (int)(FALL_DELAY_MS * SOFT_DROP_DELAY_MULT));
}

void Game::incNextStepDown(std::chrono::steady_clock::time_point last) {
    // Schedule the next step down, as an offset from last
    t_next_step_down = last + std::chrono::milliseconds(FALL_DELAY_MS);
}

void Game::handleEvent(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        // Ignore repeated keys, we implement our own repeated inputs
        if (!e.key.repeat) {
            switch (e.key.keysym.sym) {
            case SDLK_RIGHT:
                active.moveRight(&playfield);
                break;
            case SDLK_LEFT:
                active.moveLeft(&playfield);
                break;
            case SDLK_DOWN:
                startSoftDropping();
                break;
            case SDLK_UP:
                active.rotateClockw(&playfield);
                break;
            case SDLK_c:
                active.rotateCounterclockw(&playfield);
                break;
            case SDLK_SPACE:
                active.hardDrop(&playfield);
                active.lockDownAndRespawn(randomTetrominoType(), &playfield);
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