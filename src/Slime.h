#ifndef SLIME_H
#define SLIME_H
#include "Monster.h"
#include "SpriteSheet.h"
class Animator;
class Slime : public Monster
{
  public:
    Slime(float centerX, float centerY, SpriteSheet* spriteSheet,
          b2World* world);
    ~Slime();

    void render(float deltaTime) override;

  private:
    void updateGraphics(float deltaTime);
    void updatePhysics();
    void updateLogic(float deltaTime);
    enum
    {
        STATE_IDLE,
        STATE_WAIT,
        STATE_MOVE,
        STATE_ATTACK,
        STATE_HURT,
        STATE_DIE
    };
    enum
    {
        ANIM_IDLE,
        ANIM_MOVE,
        ANIM_ATTACK,
        ANIM_HURT,
        ANIM_DIE,
        NUM_ANIMS
    };

    int m_state;
    float m_timer;
    int m_direction;
    int m_originX;
    int m_originY;

    b2Body* m_body;
    Animator* m_animator;
};
#endif // SLIME_H
