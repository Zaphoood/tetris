#include "hud.h"

HUD::HUD() {}
HUD::HUD(const std::array<TetrominoKind_t, QUEUE_LEN> &queue) {
    setQueue(queue);
}

void HUD::setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue) {
    for (int i = 0; i < QUEUE_LEN; i++) {
        if (m_queue_visuals[i].getKind() != queue[i]) {
            m_queue_visuals[i].setKind(queue[i]);
        }
    }
}

void HUD::draw(SDL_Renderer *renderer) {
    for (int i = 0; i < QUEUE_LEN; i++) {
        m_queue_visuals[i].draw(renderer, QUEUE_X,
                                QUEUE_Y + QUEUE_OFFSET_Y * i);
    }
}