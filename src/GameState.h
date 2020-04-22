#ifndef GAME_STATE_H
#define GAME_STATE_H
class GameState
{
  public:
    GameState(); 
    virtual ~GameState();
    virtual void render(float deltaTime) = 0;
    virtual void show();
    virtual void hidden();
    virtual void pause();
    virtual void resume();
};
#endif // GAME_STATE_H
