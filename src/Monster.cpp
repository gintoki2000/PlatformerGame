#include "Monster.h"
#include "Constances.h"
#include "Enums.h"
#include "GameObject.h"

Monster::Monster(int monsterType, int hitPoints) :
    GameObject(GAME_OBJECT_TYPE_MONSTER),
    m_monsterType(monsterType),
    m_hitPoints(hitPoints)
{
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
	return 999.f;
}

int Monster::getFacingPlayerDirection()
{
	return DIRECTION_LEFT;
}
