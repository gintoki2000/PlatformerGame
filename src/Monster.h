#ifndef MONSTER_H
#define MONSTER_H
#include "GameObject.h"
#include "box2d/box2d.h"
class Level;
class Monster : public GameObject
{
  public:
    virtual void getHit(int damage);
    bool         isDead() const { return m_hitPoints == 0; }

    /// setters && getters
    int getMonsterType() const { return m_monsterType; }
    int getHitPoints() const { return m_hitPoints; }

  private:
    int m_monsterType;

  protected:
	Monster();
	virtual bool init(int monsterType, Level* level, int hitPoints);
    float getDistanceToPlayer();
    int   getFacingPlayerDirection();
    int   m_hitPoints;
};
#endif // MONSTER_H
