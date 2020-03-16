#ifndef GAME_STATE_H
#define GAME_STATE_H
class GameState
{
  public:
    GameState() {}
    virtual ~GameState() {}

    virtual void render(float deltaTime) = 0;
    virtual void show() = 0;
    virtual void hidden() = 0;
    virtual void paused() = 0;
    virtual void resume() = 0;
};
#endif // GAME_STATE_H
