#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"
#include "Level.h"
#include "Player.h"
#include <cmath>

Monster::Monster() : m_hitPoints(0) {}
bool Monster::init(int monsterType, Level* level, int hitPoints)
{
    GameObject::init(GAME_OBJECT_TYPE_MONSTER, level);
    m_monsterType = monsterType;
    m_hitPoints   = hitPoints;
    return true;
}
void Monster::getHit(int damage)
{
    if (!isDead())
    {
        m_hitPoints -= damage;
        if (m_hitPoints < 0)
        {
            m_hitPoints = 0;
        }
    }
}

float Monster::getDistanceToPlayer()
{
    float playerX = m_level->getPlayer()->getPositionX();
    float playerY = m_level->getPlayer()->getPositionY();
    float a       = playerX - m_positionX;
    float b       = playerY - m_positionY;
    return std::sqrt(a * a + b * b);
}

int Monster::getFacingPlayerDirection()
{
    float playerX = m_level->getPlayer()->getPositionX();
    if (playerX < m_positionX)
    {
        return DIRECTION_LEFT;
    }
    if (playerX > m_positionY)
    {
        return DIRECTION_RIGHT;
    }
    return DIRECTION_NONE;
}
