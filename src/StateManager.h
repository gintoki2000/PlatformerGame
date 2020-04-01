#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H
class GameState;
class StateManager
{
  public:
    StateManager();
    ~StateManager();

    void setState(GameState* state);
    void pushState(GameState* state);
    void popState();

	void update();
    GameState* getState() const;

  private:
    const static int MAX_STATES  = 10;
    const static int MAX_PENDING = 5;
    enum ActionType
    {
        ACTION_TYPE_PUSH,
        ACTION_TYPE_POP,
        ACTION_TYPE_CHANGE
    };
    struct Action
    {
        Action() : type(ACTION_TYPE_PUSH), state(nullptr) {}
        Action(GameState* state, ActionType type) : type(type), state(state) {}

        ActionType type;
        GameState* state;
    };
    GameState* m_stateStack[MAX_STATES];
    int        m_numStates;
    Action     m_actions[MAX_PENDING];
    int        m_numPending;
};
#endif // STATE_MANAGER_H
