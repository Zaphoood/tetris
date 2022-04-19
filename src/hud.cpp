#include <filesystem>
#include <iostream>
#include <string>

#include "hud.h"

HUD::HUD(const std::string& assets_path, const ScoringSystem& p_scoring) : m_scoring(p_scoring) {
    TTF_Init();
    std::string font_path = assets_path + "/futura-medium.ttf"; 
    m_font = TTF_OpenFont(font_path.c_str(), 20);
    if (!m_font) {
        std::cout << "ERROR: Could not load font.\n";
        exit(1);
    }
    // Force re-render on first frame by settings impossible values
    m_last_level = -1;
    m_last_goal = -1;
    m_last_lines = -1;
    m_last_score = -1;
}

HUD::HUD(const std::string& assets_path, const ScoringSystem& scoring,
         const std::array<TetrominoKind_t, QUEUE_LEN> &queue)
    : HUD(assets_path, scoring) {
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

/**
 * Re-render the Surface containing the level info if it is needed,
 * that is if the level has changed since the last render.
 */
void HUD::renderLevel(SDL_Renderer *renderer) {
    int level = m_scoring.getLevel();
    if (level == m_last_level) {
        return;
    }
    m_last_level = level;
    // TODO: Maybe make this faster (e. g. by using C++20's std::format)
    std::string text = "Level: " + std::to_string(level);
    renderText(renderer, LEVEL_TEXT_X, LEVEL_TEXT_Y, text.c_str(), m_font,
               &m_level_texture, &m_level_rect, TEXT_COLOR);
}

/**
 * Re-render the Surface containing the goal info if needed.
 */
void HUD::renderGoal(SDL_Renderer *renderer) {
    int goal = m_scoring.getGoal();
    if (goal == m_last_goal) {
        return;
    }
    m_last_goal = goal;
    std::string text = "Goal: " + std::to_string(goal);
    renderText(renderer, GOAL_TEXT_X, GOAL_TEXT_Y, text.c_str(), m_font,
               &m_goal_texture, &m_goal_rect, TEXT_COLOR);
}

void HUD::renderScore(SDL_Renderer *renderer) {
    /**
     * Re-render the Surface containing the score info if needed.
     */

    int score = m_scoring.getScore();
    if (score == m_last_score) {
        return;
    }
    m_last_score = score;
    std::string text = "Score: " + std::to_string(score);
    renderText(renderer, SCORE_TEXT_X, SCORE_TEXT_Y, text.c_str(), m_font,
               &m_score_texture, &m_score_rect, TEXT_COLOR);
}

void HUD::renderLines(SDL_Renderer *renderer) {
    /**
     * Re-render the Surface containing the lines info if needed.
     */

    int lines = m_scoring.getLines();
    if (lines == m_last_lines) {
        return;
    }
    m_last_lines = lines;
    std::string text = "Lines: " + std::to_string(lines);
    renderText(renderer, LINES_TEXT_X, LINES_TEXT_Y, text.c_str(), m_font,
               &m_lines_texture, &m_lines_rect, TEXT_COLOR);
}

void HUD::renderAll(SDL_Renderer *renderer) {
    renderLevel(renderer);
    renderGoal(renderer);
    renderScore(renderer);
    renderLines(renderer);
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
    SDL_RenderCopy(renderer, m_score_texture, 0, &m_score_rect);
    SDL_RenderCopy(renderer, m_lines_texture, 0, &m_lines_rect);
}
