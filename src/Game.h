#ifndef GAME_H
#define GAME_H
#include "SDL.h"
class GameState;
class Game
{
  public:
    Game();

    ~Game();

    bool initialize();

    void render(float dt);

	void stop();

    bool isRunning() const { return m_isRunning; }

    static Game* getInstance() { return instance; }

  private:
    bool m_isRunning;
    GameState* m_state;

    static Game* instance;
};

#endif // GAME_H
