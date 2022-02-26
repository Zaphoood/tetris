#include "cstdlib"

#include "constants.h"
#include "game.h"

Game::Game() {
    playfield = Playfield();
    active = Active(randomTetrominoType());
}

void Game::init() {
    // Start the timer for the next Tetromino
    std::chrono::steady_clock::time_point t_next_step_down =
        std::chrono::steady_clock::now() +
        std::chrono::milliseconds(FALL_DELAY_MS);
}

void Game::update() {
    if (std::chrono::steady_clock::now() > t_next_step_down) {
        t_next_step_down = std::chrono::steady_clock::now() +
                           std::chrono::milliseconds(FALL_DELAY_MS);
        if (!active.stepDown(&playfield)) {
            active.lockDownAndRespawn(randomTetrominoType(), &playfield);
            // If possible, step down immediatly after spawning (this is
            // according to the Tetris Guideline)
            active.stepDown(&playfield);
        }
    }
}

void Game::handleEvent(const SDL_Event &e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_RIGHT:
            active.moveRight(&playfield);
            break;
        case SDLK_LEFT:
            active.moveLeft(&playfield);
            break;
        case SDLK_DOWN:
            active.stepDown(&playfield);
            break;
        case SDLK_UP:
            active.rotateClockw(&playfield);
            break;
        case SDLK_c:
            active.rotateCounterclockw(&playfield);
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