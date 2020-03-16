#ifndef MONSTER_H
#define MONSTER_H
#include "GameObject.h"
#include "box2d/box2d.h"
class Monster : public GameObject
{
  public:
    Monster(int monsterType, int hitPoints);

    virtual void getHit();

    int getMonsterType() const { return m_monsterType; }

    bool isDead() const { return m_hitPoints == 0; }

    static b2Body* createBody(b2World* world, int left, int top, int width,
                              int height);

  private:
    int m_monsterType;

  protected:
    int m_hitPoints;
};
#endif // MONSTER_H
