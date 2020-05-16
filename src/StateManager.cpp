#include "StateManager.h"
#include "GameState.h"
#include "SDL.h"

StateManager::StateManager()
{
    m_numStates  = 0;
    m_numPending = 0;
}

StateManager::~StateManager() {
	Update();
	for (int i = 0; i < m_numStates; ++i)
	{
		delete m_stateStack[i];
		m_stateStack[i] = nullptr;
	}
	m_numStates = 0;
}

void StateManager::PushState(GameState* state)
{
    SDL_assert(m_numPending < MAX_PENDING);
    m_actions[m_numPending++] = Action(state, ACTION_TYPE_PUSH);
}

void StateManager::PopState()
{
    SDL_assert(m_numPending < MAX_PENDING);
    m_actions[m_numPending++] = Action(nullptr, ACTION_TYPE_POP);
}

void StateManager::SetState(GameState* state)
{
    SDL_assert(m_numPending < MAX_PENDING);
    m_actions[m_numPending++] = Action(state, ACTION_TYPE_CHANGE);
}

void StateManager::Update()
{
    if (m_numPending == 0)
        return;
    for (int i = 0; i < m_numPending; ++i)
    {
        switch (m_actions[i].type)
        {
        case ACTION_TYPE_PUSH:
        {
            SDL_assert(m_numStates < MAX_STATES);
            m_stateStack[m_numStates++] = m_actions[i].state;
            break;
        }
        case ACTION_TYPE_POP:
        {
            SDL_assert(m_numStates > 0);
            auto topState = m_stateStack[m_numStates - 1];
            delete topState;
            --m_numStates;
        }
        break;
        case ACTION_TYPE_CHANGE:
        {
            auto topState = m_stateStack[m_numStates - 1];
            delete topState;
            m_stateStack[m_numStates - 1] = m_actions[i].state;
        }
        }
    }
	m_numPending = 0;
}

GameState* StateManager::GetState() const 
{
	if (m_numStates == 0)
		return nullptr;
	return m_stateStack[m_numStates - 1];
}
