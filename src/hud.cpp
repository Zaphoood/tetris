#include "hud.h"
#include <iostream>

HUD::HUD() {}
HUD::HUD(const std::array<TetrominoKind_t, QUEUE_LEN> &queue) {
    setQueue(queue);
}

void HUD::setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue) {
    for (int i = 0; i < QUEUE_LEN; i++) {
        m_queue_visuals[i].setKind(queue[i]);
    }
}

void HUD::setHold(TetrominoKind_t held) {
    m_hold_visual.setKind(held);
}

void HUD::draw(SDL_Renderer *renderer) {
    // Draw queue
    for (int i = 0; i < QUEUE_LEN; i++) {
        m_queue_visuals[i].draw(renderer, QUEUE_X,
                                QUEUE_Y + QUEUE_OFFSET_Y * i);
    }
    // Draw hold
    m_hold_visual.draw(renderer, HOLD_X, HOLD_Y);
}