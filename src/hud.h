#include <array>
#include <string>

#include "SDL_ttf.h"

#include "tetrovis.h"

class HUD {
  private:
    // Visual representation of Tetromino queue
    std::array<TetroVisual, QUEUE_LEN> m_queue_visuals;
    // Visual representation of hold Tetromino
    TetroVisual m_hold_visual;

    // Pointers to Game's gameplay info
    const int *mp_level;

    // Font
    TTF_Font *m_font;
    SDL_Color m_text_color{0, 0, 0, 0};
    int m_last_level;
    SDL_Texture *m_level_texture;
    SDL_Rect m_level_rect;
    void renderLevel(SDL_Renderer *renderer);
    void renderText(SDL_Renderer *renderer, int x, int y, const char *text,
                    TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
                    const SDL_Color &text_color);

  public:
    HUD(const int *p_level);
    HUD(const int *p_level,
        const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    ~HUD();

    void setQueue(const std::array<TetrominoKind_t, QUEUE_LEN> &queue);
    void setHold(TetrominoKind_t hold);
    void draw(SDL_Renderer *renderer);
};