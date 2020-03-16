#include "StateManager.h"
#include "GameState.h"

StateManager::StateManager() { m_state = nullptr; }

StateManager::~StateManager() { delete m_state; }

void StateManager::setState(GameState* state)
{
    if (m_state != nullptr)
    {
        delete m_state;
        m_state = nullptr;
    }
    m_state = state;
}

GameState* StateManager::getState() const { return m_state; }
