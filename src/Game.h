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

    bool Init();
    void Render(float dt);
    void Stop();
    bool IsRunning() const { return m_isRunning; }

    StateManager&   GetStateManager() const { return *m_stateMGR; }
    SDL_Renderer*   GetRenderer() const { return m_renderer; }
    SDL_Window*     GetWindow() const { return m_window; }

    static Game* GetInstance() { return instance; }

  private:
    bool            m_isRunning;
    StateManager*   m_stateMGR;
    SDL_Renderer*   m_renderer;
    SDL_Window*     m_window;

    static Game* instance;
};

#define GAME Game::GetInstance()

#endif // GAME_H
