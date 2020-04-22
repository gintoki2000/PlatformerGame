#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "SDL_render.h"
class GameState;
class StateManager;
class TextureManager;
class SoundManager;
class WorldManager;
class Game
{
  public:
    Game();
    ~Game();

    bool init();
    void render(float dt);
    void stop();
    bool isRunning() const { return m_isRunning; }

    StateManager&   stateMGR() const { return *m_stateMGR; }
    TextureManager& textureMGR() const { return *m_textureMGR; }
    SoundManager&   soundMGR() const { return *m_soundMGR; }
    SDL_Renderer*   renderer() const { return m_renderer; }
    SDL_Window*     window() const { return m_window; }

    static Game* getInstance() { return instance; }

  private:
    bool            m_isRunning;
    StateManager*   m_stateMGR;
    TextureManager* m_textureMGR;
    SoundManager*   m_soundMGR;
    SDL_Renderer*   m_renderer;
    SDL_Window*     m_window;

    static Game* instance;
};

#endif // GAME_H
