#ifndef TITLE_STATE_H
#define TITLE_STATE_H
#include "GameState.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
class TitleState : public GameState
{
  public:
    static TitleState* create();
    ~TitleState();

    void render(float deltaTime) override;

  private:
    TitleState();
    bool init();
    void handle(int op);
    void drawToCenter(SDL_Texture* text, int startY, int index, int margin);
    static constexpr int NUM_OPTIONS = 1;
    SDL_Texture*         m_unselectedTexts[NUM_OPTIONS];
    SDL_Texture*         m_selectedTexts[NUM_OPTIONS];
    int                  m_selected;
};
#endif // TITLE_STATE_H
