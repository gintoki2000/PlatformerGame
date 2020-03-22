#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"

Monster::Monster() : m_hitPoints(0) {}
bool Monster::init(int monsterType, Level* level, int hitPoints)
{
	GameObject::init(GAME_OBJECT_TYPE_MONSTER, level);
	m_monsterType = monsterType;
	m_hitPoints = hitPoints;
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

float Monster::getDistanceToPlayer() { return 999.f; }

int Monster::getFacingPlayerDirection() { return DIRECTION_LEFT; }
