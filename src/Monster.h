#ifndef MONSTER_H
#define MONSTER_H
#include "GameObject.h"
class Monster : public GameObject
{
  public:
    enum Type
    {
        SMLIE
    };

    Monster(Type monsterType) :
        GameObject(GameObject::Type::MONSTER), m_type(monsterType)
    {
    }

    Type getMonsterType() { return m_type; }

    virtual void getDamage(int damage){};

  private:
    Type m_type;
};
#endif // MONSTER_H
