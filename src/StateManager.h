#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
class GameState;
class StateManager
{
  public:
    StateManager();

    ~StateManager();

    void setState(GameState* state);
    GameState* getState() const;

  private:
    GameState* m_state;
};
#endif // STATE_MANAGER_H
