#include <filesystem>
#include <iostream>
#include <string>

#include "constants.h"
#include "hud.h"

HUD::HUD(const std::string &assets_path, const ScoringSystem &p_scoring)
    : m_scoring(p_scoring) {
    TTF_Init();
    std::string font_path = assets_path + FONT_PATH_RELATIVE;
    m_font = TTF_OpenFont(font_path.c_str(), FONT_SIZE);
    if (!m_font) {
        std::cout << "ERROR: Failed to load font from " << font_path << "\n";
        exit(1);
    }
    reset();
}

HUD::HUD(const std::string &assets_path, const ScoringSystem &scoring,
         const std::array<TetrominoKind_t, QUEUE_LEN> &queue)
    : HUD(assets_path, scoring) {
    setQueue(queue);
}

HUD::~HUD() {
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_score_texture);
    SDL_DestroyTexture(m_level_texture);
    SDL_DestroyTexture(m_goal_texture);
    SDL_DestroyTexture(m_lines_texture);
    m_font = NULL;
}

void HUD::reset() {
    // Force re-render on first frame of new game by setting impossible values
    m_last_level = -1;
    m_last_goal = -1;
    m_last_lines = -1;
    m_last_score = -1;
    setHold(-1);
}

void HUD::setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue) {
    for (int i = 0; i < QUEUE_LEN; i++) {
        m_queue_visuals[i].setKind(queue[i]);
    }
}

void HUD::setHold(TetrominoKind_t held) {
    m_hold_visual.setKind(held);
}

void HUD::draw(SDL_Renderer *renderer, GameState state) {
    // Draw queue
    for (int i = 0; i < QUEUE_LEN; i++) {
        m_queue_visuals[i].draw(renderer, QUEUE_X,
                                QUEUE_Y + QUEUE_OFFSET_Y * i);
    }
    // Draw hold
    m_hold_visual.draw(renderer, HOLD_X, HOLD_Y);
    // Render info strings onto surfaces
    renderAllInfo(renderer);
    // Draw info
    drawAllInfo(renderer);

    if (state == GameState::Paused) {
        drawPauseOverlay(renderer);
    } else if (state == GameState::GameOver) {
        // Draw Game Over screen
        drawGameOverOverlay(renderer);
    }
}

void HUD::drawPauseOverlay(SDL_Renderer *renderer) {
    // SDL_Rect full_screen{0, 0, WINDOW_X, WINDOW_Y};
    SDL_Rect overlay_rect{PLAYFIELD_DRAW_X, PLAYFIELD_DRAW_Y,
                          PLAYFIELD_WIDTH + 1, PLAYFIELD_HEIGHT + 1};
    uint8_t val = 0;
    SDL_SetRenderDrawColor(renderer, val, val, val, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &overlay_rect);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    if (!m_paused_texture) {
        renderPaused(renderer);
    }
    SDL_RenderCopy(renderer, m_paused_texture, 0, &m_paused_rect);
}

void HUD::drawGameOverOverlay(SDL_Renderer *renderer) {
    // Fill screen with dark, transparent color
    SDL_Rect full_screen{0, 0, WINDOW_X, WINDOW_Y};
    uint8_t val = 0;
    SDL_SetRenderDrawColor(renderer, val, val, val, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &full_screen);
    // Render text so that we know the measurements
    if (!m_game_over_texture) {
        renderGameOver(renderer);
    }
    // Draw a box around the text
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, BACKGROUND.r, BACKGROUND.g, BACKGROUND.b,
                           BACKGROUND.a);
    int margin = 20;
    SDL_Rect rect{m_game_over_rect.x - margin, m_game_over_rect.y - margin,
                  m_game_over_rect.w + margin * 2,
                  m_game_over_rect.h + margin * 2};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, TEXT_COLOR.r, TEXT_COLOR.g, TEXT_COLOR.b,
                           TEXT_COLOR.a);
    SDL_RenderDrawRect(renderer, &rect);
    // Draw the text
    SDL_RenderCopy(renderer, m_game_over_texture, 0, &m_game_over_rect);
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

/**
 * Re-render the Surface containing the score info if needed.
 */
void HUD::renderScore(SDL_Renderer *renderer) {
    int score = m_scoring.getScore();
    if (score == m_last_score) {
        return;
    }
    m_last_score = score;
    std::string text = "Score: " + std::to_string(score);
    renderText(renderer, SCORE_TEXT_X, SCORE_TEXT_Y, text.c_str(), m_font,
               &m_score_texture, &m_score_rect, TEXT_COLOR);
}

/**
 * Re-render the Surface containing the lines info if needed.
 */
void HUD::renderLines(SDL_Renderer *renderer) {
    int lines = m_scoring.getLines();
    if (lines == m_last_lines) {
        return;
    }
    m_last_lines = lines;
    std::string text = "Lines: " + std::to_string(lines);
    renderText(renderer, LINES_TEXT_X, LINES_TEXT_Y, text.c_str(), m_font,
               &m_lines_texture, &m_lines_rect, TEXT_COLOR);
}

void HUD::renderAllInfo(SDL_Renderer *renderer) {
    renderLevel(renderer);
    renderGoal(renderer);
    renderScore(renderer);
    renderLines(renderer);
}

/**
 * Render the Surface containing the 'Paused' text.
 */
void HUD::renderPaused(SDL_Renderer *renderer) {
    // Use dummy value for x-position since it'll be changed afterwards
    renderText(renderer, 0, PAUSED_TEXT_Y, "Paused", m_font, &m_paused_texture,
               &m_paused_rect, TEXT_COLOR, TextRenderMode::BLENDED);
    // Center rect horizontally
    m_paused_rect.x =
        PLAYFIELD_DRAW_X + (PLAYFIELD_WIDTH - m_paused_rect.w) / 2;
}

/**
 * Render the Surface containing the 'Game Over' text.
 */
void HUD::renderGameOver(SDL_Renderer *renderer) {
    // Use dummy value for x-position since it'll be changed afterwards
    renderText(renderer, 0, PAUSED_TEXT_Y, "Game Over", m_font,
               &m_game_over_texture, &m_game_over_rect, TEXT_COLOR,
               TextRenderMode::BLENDED);
    // Center rect horizontally
    m_game_over_rect.x =
        PLAYFIELD_DRAW_X + (PLAYFIELD_WIDTH - m_game_over_rect.w) / 2;
}

/**
 * Render text onto a texture using the `Shaded` text rendering mode and set
 * the bounding rect.
 */
void HUD::renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                     TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                     const SDL_Color &text_color) {
    renderText(renderer, x, y, text, font, texture, rect, text_color,
               TextRenderMode::SHADED);
}

void HUD::renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                     TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                     const SDL_Color &text_color, const TextRenderMode mode) {
    SDL_Surface *surface;
    switch (mode) {
    case TextRenderMode::SHADED:
        surface = TTF_RenderText_Shaded(font, text, text_color, BACKGROUND);
        break;
    case TextRenderMode::BLENDED:
        surface = TTF_RenderText_Blended(font, text, text_color);
        break;
    default:
        std::cout
            << "Exhaustive handling of TextRenderMode in HUD::renderText()\n";
        exit(1);
    }
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

void HUD::drawAllInfo(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer, m_level_texture, 0, &m_level_rect);
    SDL_RenderCopy(renderer, m_goal_texture, 0, &m_goal_rect);
    SDL_RenderCopy(renderer, m_score_texture, 0, &m_score_rect);
    SDL_RenderCopy(renderer, m_lines_texture, 0, &m_lines_rect);
}
