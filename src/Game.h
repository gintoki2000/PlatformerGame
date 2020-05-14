#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "SDL_render.h"
class GameState;
class StateManager;
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
    SDL_Renderer*   renderer() const { return m_renderer; }
    SDL_Window*     window() const { return m_window; }

    static Game* getInstance() { return instance; }

  private:
    bool            m_isRunning;
    StateManager*   m_stateMGR;
    SDL_Renderer*   m_renderer;
    SDL_Window*     m_window;

    static Game* instance;
};

#define GAME Game::getInstance()

#endif // GAME_H
