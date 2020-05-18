#ifndef GAME_STATE_H
#define GAME_STATE_H
class GameState
{
  public:
    GameState(); 
    virtual ~GameState();
    virtual void Render(float deltaTime) = 0;
    virtual void Show();
    virtual void Hidden();
    virtual void Pause();
    virtual void Resume();
};
#endif // GAME_STATE_H
