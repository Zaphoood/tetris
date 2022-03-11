#include <array>
#include <string>

#include "SDL_ttf.h"

#include "scoring.h"
#include "tetrovis.h"

class HUD {
  private:
    // Visual representation of Tetromino queue
    std::array<TetroVisual, QUEUE_LEN> m_queue_visuals;
    // Visual representation of hold Tetromino
    TetroVisual m_hold_visual;

    // Pointers to Game's gameplay info
    const ScoringSystem *mp_scoring;

    // Font
    TTF_Font *m_font;
    SDL_Color m_text_color{0, 0, 0, 0};
    // Store last value of level so that we know when to re-render
    int m_last_level;
    SDL_Texture *m_level_texture;
    SDL_Rect m_level_rect;
    void renderLevel(SDL_Renderer *renderer);
    // Same as m_last_level
    int m_last_goal;
    SDL_Texture *m_goal_texture;
    SDL_Rect m_goal_rect;
    void renderGoal(SDL_Renderer *renderer);
    void renderAll(SDL_Renderer *renderer);
    void renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                    TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                    const SDL_Color &text_color);
    void displayAll(SDL_Renderer *renderer);

  public:
    HUD(const ScoringSystem *p_scoring);
    HUD(const ScoringSystem *p_scoring,
        const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    ~HUD();

    void setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    void setHold(TetrominoKind_t hold);
    void draw(SDL_Renderer *renderer);
};