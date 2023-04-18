#include <array>
#include <string>

#include "SDL_ttf.h"

#include "constants.h"
#include "scoring.h"
#include "tetrovis.h"

enum class TextRenderMode { SHADED, BLENDED };

class HUD {
  private:
    // Visual representation of Tetromino queue
    std::array<TetroVisual, QUEUE_LEN> m_queue_visuals;
    // Visual representation of hold Tetromino
    TetroVisual m_hold_visual;

    // Game classes's gameplay info
    const ScoringSystem &m_scoring;

    // Font
    TTF_Font *m_font;
    // Store last value of level, goal etc. so that we know when to re-render
    int m_last_level;
    SDL_Texture *m_level_texture;
    SDL_Rect m_level_rect;
    void renderLevel(SDL_Renderer *renderer);

    int m_last_goal;
    SDL_Texture *m_goal_texture;
    SDL_Rect m_goal_rect;
    void renderGoal(SDL_Renderer *renderer);

    int m_last_lines;
    SDL_Texture *m_lines_texture;
    SDL_Rect m_lines_rect;
    void renderLines(SDL_Renderer *renderer);

    int m_last_score;
    SDL_Texture *m_score_texture;
    SDL_Rect m_score_rect;
    void renderScore(SDL_Renderer *renderer);

    void renderAllInfo(SDL_Renderer *renderer);

    SDL_Texture *m_paused_texture = nullptr;
    SDL_Rect m_paused_rect;
    int m_paused_x, m_paused_y;
    void renderPaused(SDL_Renderer *renderer);

    SDL_Texture *m_game_over_texture = nullptr;
    SDL_Rect m_game_over_rect;
    int m_game_over_x, m_game_over_y;
    void renderGameOver(SDL_Renderer *renderer);

    void renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                    TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                    const SDL_Color &text_color);
    void renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                    TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                    const SDL_Color &text_color, const TextRenderMode mode);
    void drawAllInfo(SDL_Renderer *renderer);

    void drawPauseOverlay(SDL_Renderer *renderer);
    void drawGameOverOverlay(SDL_Renderer *renderer);

  public:
    HUD(const std::string &assets_path, const ScoringSystem &p_scoring);
    HUD(const std::string &assets_path, const ScoringSystem &p_scoring,
        const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    ~HUD();

    void reset();

    void setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    void setHold(TetrominoKind_t hold);
    void draw(SDL_Renderer *renderer, GameState state);
};
