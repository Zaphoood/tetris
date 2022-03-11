#include <filesystem>
#include <iostream>
#include <string>

#include "hud.h"

HUD::HUD(const ScoringSystem *p_scoring) : mp_scoring(p_scoring) {
    TTF_Init();
    m_font = TTF_OpenFont("./assets/futura-medium.ttf", 20);
    if (!m_font) {
        std::cout << "ERROR: Could not load font.\n";
        exit(1);
    }
}

HUD::HUD(const ScoringSystem *p_scoring,
         const std::array<TetrominoKind_t, QUEUE_LEN> &queue)
    : HUD(p_scoring) {
    setQueue(queue);
}

HUD::~HUD() {
    TTF_CloseFont(m_font);
    m_font = NULL;
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

    // Draw info
    renderAll(renderer);
    displayAll(renderer);
}

void HUD::renderLevel(SDL_Renderer *renderer) {
    /**
     * Re-render the Surface containing the level info *only* if it is needed,
     * that is if the level has changed since the last render.
     */
    int level = mp_scoring->getLevel();
    if (level == m_last_level) {
        return;
    }
    m_last_level = level;
    // TODO: Maybe make this faster (e. g. by using C++20's std::format)
    std::string text = "Level: " + std::to_string(level);
    renderText(renderer, LEVEL_TEXT_X, LEVEL_TEXT_Y, text.c_str(), m_font,
               &m_level_texture, &m_level_rect, TEXT_COLOR);
}

void HUD::renderGoal(SDL_Renderer *renderer) {
    /**
     * Re-render the Surface containing the goal info if needed.
     * (Same as renderLevel())
     */

    int goal = mp_scoring->getGoal();
    if (goal == m_last_goal) {
        return;
    }
    m_last_goal = goal;
    // TODO: Maybe make this faster (e. g. by using C++20's std::format)
    std::string text = "Goal: " + std::to_string(goal);
    renderText(renderer, GOAL_TEXT_X, GOAL_TEXT_Y, text.c_str(), m_font,
               &m_goal_texture, &m_goal_rect, TEXT_COLOR);
}

void HUD::renderAll(SDL_Renderer *renderer) {
    renderLevel(renderer);
    renderGoal(renderer);
}

void HUD::renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                     TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                     const SDL_Color &text_color) {
    /**
     * Render text onto a texture and set the bounding rect.
     */
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, text_color);
    if (!surface) {
        std::cout << "ERROR: Couldn't create surface\n";
        exit(1);
    }
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
}

void HUD::displayAll(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, m_level_texture, 0, &m_level_rect);
    SDL_RenderCopy(renderer, m_goal_texture, 0, &m_goal_rect);
}