#include "Slime.h"
#include "Animator.h"
#include "Constances.h"
#include "Locator.h"
void Slime::updateGraphics(float deltaTime) { m_animator->update(deltaTime); }

void Slime::updatePhysics()
{
    const auto& position = m_body->GetPosition();
    setPosition(position.x * Constances::PPM, position.y * Constances::PPM);
}

void Slime::updateLogic(float deltaTime)
{
    switch (m_state)
    {
    case STATE_IDLE:
    {
    }
    break;
    case STATE_WAIT:
    {
    }
    break;
    case STATE_MOVE:
    {
    }
    break;
    case STATE_ATTACK:
    {
    }
    break;
    case STATE_HURT:
    {
    }
    break;
    case STATE_DIE:
    {
    }
    break;
    }
}

void Slime::render(float deltaTime)
{
    updatePhysics();
    updateGraphics(deltaTime);
    updateLogic(deltaTime);

    m_animator->render(Locator::getRenderer(), getPositionX(), getPositionY());
}
