#ifndef SPELL_H
#define SPELL_H
#include "Animation.h"
#include "Enums.h"
#include "GameObject.h"
#include "box2d/box2d.h"
class Player;
class Spell : public GameObject, public b2QueryCallback
{
  public:
    Spell(Level* level);

    void tick(float deltaTime) override;
    void paint() override;

  protected:
    virtual void checkCollision();

    bool ReportFixture(b2Fixture* fixture) override;

    int        m_target;
    int        m_damage;
    int        m_width;
    int        m_height;
    Direction  m_direction;
    Animation* m_animation;
    b2Body*    m_body;
    float      m_elpasedTime;
};
#endif // SPELL_H
